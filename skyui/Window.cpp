#include "Window.h"
#include "Pixmap.h"
#include "Widget.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace {
    jlib::MouseButton mapMouseButton(unsigned int b) {
        switch (b) {
            case Button1: return jlib::MouseButton::LEFT;
            case Button2: return jlib::MouseButton::MIDDLE;
            case Button3: return jlib::MouseButton::RIGHT;
            default: throw std::runtime_error("process mouse error");
        }
    }
}

namespace jlib {

class Window::WindowPrivateImpl {
public:
    using Relation = TreeNode<SkyWidget>;
    WindowPrivateImpl();
    ~WindowPrivateImpl();
    void move(int x, int y);
    void setTitle(const std::string &title);
    void setGeometry(int x, int y, size_t w, size_t h);
    void resize(size_t w, size_t h);
    void show();
    void flush();
    void loop();
    void compose();
    void dispatch(const Event &e);
    void push(const SkyWidget &widget);
    void remove(const SkyWidget &widget);
    void relate(const SkyWidget &parent, const SkyWidget &child);
    void onMouseMove(const MouseMoveEvent &e);
    void onMousePress(const MousePressEvent &e);
    void onMouseRelease(const MouseReleaseEvent &e);
    void onMouseWheel(const MouseWheelEvent &e);
    void onKeyPress(const KeyPressEvent &e);
    void onKeyRelease(const KeyReleaseEvent &e);
    void onResize(const jlib::WindowResizeEvent &e);
    SkyWidget locate(const Point& p);
    Size size() const;
    Point pos() const;
    std::string title() const;
private:
    SkyWidget base_;
    std::vector<SkyWidget> stack_;
    int x_, y_;
    std::string title_;
    Display *display_;
    ::Window window_, root_;
    GC gc_;
    XImage *image_;
    XVisualInfo info_;
    XSetWindowAttributes attrs_;
    SkyWidget  activeWidget_,
              pressedWidget_,
              hoveredWidget_;
    Relation::AutoTreeNode relation_;
};

Window::WindowPrivateImpl::WindowPrivateImpl()
: base_(std::make_shared<Widget>(Size(320, 180))),
  stack_(), x_(0), y_(0), title_(),
  display_(nullptr), window_(0), root_(0),
  gc_(nullptr), image_(nullptr), info_{}, attrs_{},
  pressedWidget_(nullptr), hoveredWidget_(nullptr),
  activeWidget_(nullptr), relation_(Relation::build(base_, base_->uuid())) {}

Window::WindowPrivateImpl::~WindowPrivateImpl() {
    if (display_) {
        XCloseDisplay(display_);
        display_ = nullptr;
    }
}

void Window::WindowPrivateImpl::move(int x, int y) {
    x_ = x;
    y_ = y;
    XSizeHints hints;
    hints.x = x_;
    hints.y = y_;
    hints.flags = PPosition;
    if (!display_) return;
    XSetWMNormalHints(display_, window_, &hints);
}

void Window::WindowPrivateImpl::setTitle(const std::string &title) {
    title_ = title;
    if (!display_) return;
    XStoreName(display_, window_, title_.c_str());
}

void Window::WindowPrivateImpl::setGeometry(int x, int y, size_t w, size_t h) {
    x_ = x;
    y_ = y;
    base_->resize({w, h});
}

void Window::WindowPrivateImpl::resize(size_t w, size_t h) {
    base_->resize({w, h});
    if (!display_) return;
    image_ = XCreateImage(display_, info_.visual, info_.depth, ZPixmap, 0,
                          reinterpret_cast<char *>(base_->canvas().data()),
                          base_->width(), base_->height(), 8, (int) base_->width() * 4);
    dispatch(EventUtil::buildWindowResizeEvent({w, h}));
    flush();
}

void Window::WindowPrivateImpl::push(const SkyWidget &widget) {
    stack_.emplace_back(widget);
}

void Window::WindowPrivateImpl::remove(const SkyWidget &widget) {
    stack_.erase(std::find(stack_.begin(), stack_.end(), widget));
}

void Window::WindowPrivateImpl::show() {
    if (display_) return;
    display_ = XOpenDisplay(nullptr);
    if (!display_)
        throw std::runtime_error("");
    if (!XMatchVisualInfo(display_, XDefaultScreen(display_), 24, TrueColor, &info_))
        throw std::runtime_error("");
    root_ = XDefaultRootWindow(display_);
    attrs_.colormap = XCreateColormap(display_, root_, info_.visual, AllocNone);
    window_ = XCreateWindow(display_, root_, x_, y_, base_->width(), base_->height(), 0, info_.depth,
                            InputOutput, info_.visual, CWBackPixel | CWColormap | CWBorderPixel, &attrs_);
    XSelectInput(display_, window_,
                 ExposureMask
                 | StructureNotifyMask // window resize
                 | KeyPressMask | KeyReleaseMask  // keyboard
                 | PointerMotionMask // mouse move
                 | ButtonPressMask | ButtonReleaseMask // mouse press
    );
    XGCValues values{0};
    gc_ = XCreateGC(display_, root_, GCGraphicsExposures, &values);
    if (!gc_) throw std::runtime_error("");
    image_ = XCreateImage(display_, info_.visual, info_.depth,
                          ZPixmap, 0, reinterpret_cast<char *>(base_->canvas().data()), base_->width(), base_->height(),
                          8, (int) base_->width() * 4);
    XMapWindow(display_, window_);
    move(x_, y_);
    setTitle(title_);
}

void Window::WindowPrivateImpl::loop() {
    if (!display_) return;
    XFlush(display_);
    XEvent event;
    while (!XNextEvent(display_, &event)) {
        switch (event.type) {
            case Expose: {
                flush();
                break;
            }
            case ConfigureNotify: {
                size_t w = event.xconfigure.width, h = event.xconfigure.height;
                if (base_->size() != Size(w, h)) {
//                    printf("resize : %d %d\n", w, h);
                    resize(w, h);
                }
                break;
            }
            case MotionNotify: {
                dispatch(EventUtil::buildMouseMoveEvent(event.xmotion.x, event.xmotion.y));
                break;
            }
            case KeyPress: {
                dispatch(EventUtil::buildKeyPressEvent(event.xkey.keycode));
                break;
            }
            case KeyRelease: {
                dispatch(EventUtil::buildKeyReleaseEvent(event.xkey.keycode));
                break;
            }
            case ButtonPress: {
                if (event.xbutton.button == 4) // wheel 4 : up, 5 : down
                    dispatch(EventUtil::buildMouseWheelEvent(event.xbutton.x, event.xbutton.y, 1));
                else if (event.xbutton.button == 5)
                    dispatch(EventUtil::buildMouseWheelEvent(event.xbutton.x, event.xbutton.y, -1));
                else if (event.xbutton.button < 4)
                    dispatch(EventUtil::buildMousePressEvent(event.xbutton.x, event.xbutton.y,
                                                             mapMouseButton(event.xbutton.button)));
                break;
            }
            case ButtonRelease: {
                if (event.xbutton.button < 4)
                    dispatch(EventUtil::buildMouseReleaseEvent(event.xbutton.x, event.xbutton.y,
                                                               mapMouseButton(event.xbutton.button)));
                break;
            }
            default:;
        }
    }
}

void Window::WindowPrivateImpl::flush() {
    compose();
    XPutImage(display_, window_, gc_, image_, 0, 0, 0, 0, base_->width(), base_->height());
}

void Window::WindowPrivateImpl::dispatch(const Event &e) {
    switch (e->type()) {
        case EventType::ETMouseMove:
            onMouseMove(EventUtil::asMouseMoveEvent(e));
            break;
        case EventType::ETMousePress:
            onMousePress(EventUtil::asMousePressEvent(e));
            break;
        case EventType::ETMouseRelease:
            onMouseRelease(EventUtil::asMouseReleaseEvent(e));
            break;
        case EventType::ETMouseWheel:
            onMouseWheel(EventUtil::asMouseWheelEvent(e));
            break;
        case EventType::ETKeyPress:
            onKeyPress(EventUtil::asKeyPressEvent(e));
            break;
        case EventType::ETKeyRelease:
            onKeyRelease(EventUtil::asKeyReleaseEvent(e));
            break;
        case EventType::ETWindowResize:
            onResize(EventUtil::asWindowResizeEvent(e));
            break;
        default:;
    }
}


void Window::WindowPrivateImpl::compose() {
    auto& canvas = base_->canvas();
    canvas.fill(0xFFFFFFFF);
    for (auto &widget: stack_) {
        if (!widget->isVisible()) continue;
        printf("---x1\n");
        widget->onPaint();
        auto shadow = widget->shadow();
        printf("---x2\n");
        if (shadow) {
            auto pos = shadow->pos();
            canvas.blend(shadow->shadow(), pos);
        }
        printf("---x3\n");
        if (widget->alpha()) {
            printf("---x4\n");
            canvas.blend(widget->compose(), widget->pos());
        } else {
            printf("---x4.1 %d %s\n", (int) widget->fillet(), widget->compose().to_string().c_str());
            canvas.cover(widget->compose(), widget->pos());
        }
    }
}

SkyWidget Window::WindowPrivateImpl::locate(const Point &pos) {
    for (int i = (int) stack_.size() - 1; i > -1; i--) {
        auto widget = stack_[i];
        if (widget->contains(pos)) return widget;
    }
    return nullptr;
}

Size Window::WindowPrivateImpl::size() const {
    return base_->size();
}

Point Window::WindowPrivateImpl::pos() const {
    return {x_, y_};
}

std::string Window::WindowPrivateImpl::title() const {
    return title_;
}

void Window::WindowPrivateImpl::onMouseMove(const MouseMoveEvent &e) {
    auto widget = locate(e->pos());
    if (!widget) {
        if (hoveredWidget_) {
            hoveredWidget_->onMouseLeave(e);
            flush();
//                printf("Window::WindowPrivateImpl::onMouseMove rcMouseOnView : %p, widget : %p, onMouseLeave : %p\n", rcMouseOnView, widget, rcMouseOnView);
        }
        hoveredWidget_ = nullptr;
    } else {
        if (!hoveredWidget_) {
            widget->onMouseEnter(e);
            flush();
//                printf("Window::WindowPrivateImpl::onMouseMove rcMouseOnView : %p, widget : %p, onMouseEnter : %p\n", rcMouseOnView, widget, widget);
        } else if (hoveredWidget_ != widget) {
            hoveredWidget_->onMouseLeave(e);
            widget->onMouseEnter(e);
//                printf("Window::WindowPrivateImpl::onMouseMove rcMouseOnView : %p, widget : %p, onMouseLeave : %p, onMouseEnter : %p\n",
//                       rcMouseOnView, widget, rcMouseOnView, widget);
            flush();
        }
        hoveredWidget_ = widget;
    }
}

void Window::WindowPrivateImpl::onMousePress(const MousePressEvent &e) {
    auto widget = locate(e->pos());
    if (!widget) return;
    widget->onMousePress(e);
    pressedWidget_ = widget;
    activeWidget_ = widget;
    flush();
}

void Window::WindowPrivateImpl::onMouseRelease(const MouseReleaseEvent &e) {
    auto widget = locate(e->pos());
    if (!widget) return;
    if (pressedWidget_ == widget) widget->onClick(e);
    widget->onMouseRelease(e);
    pressedWidget_ = nullptr;
    flush();
}

void Window::WindowPrivateImpl::onMouseWheel(const MouseWheelEvent &e) {
    auto widget = locate(e->pos());
    if (!widget) return;
    widget->onMouseWheel(e);
}

void Window::WindowPrivateImpl::onKeyPress(const KeyPressEvent &e) {
    if (!activeWidget_) return;
    activeWidget_->onKeyPress(e);
}

void Window::WindowPrivateImpl::onKeyRelease(const KeyReleaseEvent &e) {
    if (!activeWidget_) return;
    activeWidget_->onKeyRelease(e);
}

void Window::WindowPrivateImpl::onResize(const WindowResizeEvent &e) {
// framebuffer_ = Pixmap(e->size());
//        stack_[0]->onResize(e);
}

void Window::WindowPrivateImpl::relate(const SkyWidget &parent, const SkyWidget &child) {

}

Window::Window() : prv_(std::make_shared<WindowPrivateImpl>()) {}
Window::~Window() = default;
void Window::move(int x, int y) { prv_->move(x, y); }
void Window::setGeometry(int x, int y, size_t w, size_t h) { prv_->setGeometry(x, y, w, h); }
void Window::setTitle(const std::string &title) { prv_->setTitle(title); }
void Window::resize(size_t w, size_t h) { prv_->resize(w, h); }
void Window::show() { prv_->show(); }
void Window::loop() { prv_->loop(); }
void Window::push(const SkyWidget &widget) { prv_->push(widget); }
void Window::remove(const SkyWidget &widget) { prv_->remove(widget); }
Size Window::size() const { return prv_->size(); }
Point Window::pos() const { return prv_->pos(); }
std::string Window::title() const { return prv_->title(); }

}