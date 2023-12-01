#include "Window.h"
#include <stdexcept>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

using XWindow = ::Window;

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

class Window::PrivateWindow {
public:
    Display*    dsp_;
    XWindow     win_, root_;
    GC          gc_;
    XImage*     frame_;
    XVisualInfo info_;
    XSetWindowAttributes attrs_;
};

Window::Window(Compositor *compositor, int x, int y, const char* title)
    : compositor_(compositor), x_(x), y_(y), title_(title) {
    create();
}

Window::~Window() {
    dispose();
}

void Window::run() {
    auto &dsp_ = impl_->dsp_;
    auto &win_ = impl_->win_;
    auto &gc_ = impl_->gc_;
    auto &frame_ = impl_->frame_;
    auto &info_ = impl_->info_;
    XEvent event;
    while (!XNextEvent(dsp_, &event)) {
        switch (event.type) {
            case Expose: {
                update();
                break;
            }
            case ConfigureNotify: {
                Size new_size(event.xconfigure.width, event.xconfigure.height);
                const auto& framebuffer = compositor_->framebuffer();
                if (framebuffer.size() != new_size) {
                    printf("resize : %d %d\n", event.xconfigure.width, event.xconfigure.height);
                    compositor_->onResize(EventUtil::buildWindowResizeEvent(new_size));
                    frame_ = XCreateImage(dsp_, info_.visual, info_.depth,
                                          ZPixmap, 0, reinterpret_cast<char *>(framebuffer.data()), framebuffer.width(),
                                          framebuffer.height(), 8, (int) framebuffer.width() * 4);
                    compositor_->flush();
                }
//                dispatch(EventUtil::buildWindowResizeEvent({(size_t) event.xconfigure.width, (size_t) event.xconfigure.height}));
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
                    dispatch(EventUtil::buildMousePressEvent(event.xbutton.x, event.xbutton.y, mapMouseButton(event.xbutton.button)));
                break;
            }
            case ButtonRelease: {
                if (event.xbutton.button < 4)
                    dispatch(EventUtil::buildMouseReleaseEvent(event.xbutton.x, event.xbutton.y, mapMouseButton(event.xbutton.button)));
                break;
            }
        }
    }
}

void Window::create() {
    impl_ = std::make_shared<PrivateWindow>();
    auto &dsp_ = impl_->dsp_;
    auto &root_ = impl_->root_;
    auto &win_ = impl_->win_;
    auto &gc_ = impl_->gc_;
    auto &frame_ = impl_->frame_;
    auto &info_ = impl_->info_;
    auto &attrs_ = impl_->attrs_;
    dsp_ = XOpenDisplay(NULL);
    if (!dsp_)
        throw std::runtime_error("Fail to connect x window.");
    if (!XMatchVisualInfo(dsp_, XDefaultScreen(dsp_), 24, TrueColor, &info_))
        throw std::runtime_error("No such visual\n");
    root_ = XDefaultRootWindow(dsp_);
    attrs_.colormap = XCreateColormap(dsp_, root_, info_.visual, AllocNone);
    attrs_.background_pixel = 0;
    attrs_.border_pixel = 0;
    auto framebuffer = compositor_->framebuffer();
    win_ = XCreateWindow(dsp_, root_, x_, y_, framebuffer.width(), framebuffer.height(), 0, info_.depth,
                            InputOutput, info_.visual, CWBackPixel | CWColormap | CWBorderPixel, &attrs_);
    XStoreName(dsp_, win_, title_.c_str());
    XSelectInput(dsp_, win_,
                 ExposureMask
                 | StructureNotifyMask // window resize
                 | KeyPressMask | KeyReleaseMask  // keyboard
                 | PointerMotionMask // mouse move
                 | ButtonPressMask | ButtonReleaseMask // mouse press
                 );
//    XSelectInput(dsp_, win_, ExposureMask | KeyPressMask | PointerMotionMask);
    XGCValues values;
    values.graphics_exposures = 0;
    gc_ = XCreateGC(dsp_, root_, GCGraphicsExposures, &values);
    if (!gc_) throw std::runtime_error("error: fail to create gc.");
    frame_ = XCreateImage(dsp_, info_.visual, info_.depth,
                          ZPixmap, 0, reinterpret_cast<char *>(framebuffer.data()), framebuffer.width(), framebuffer.height(), 8, framebuffer.width() * 4);
//    printf("- create frame %p\n", framebuffer_);
    XSizeHints hints;
    hints.x = x_;
    hints.y = y_;
    hints.flags = PPosition;
    XSetWMNormalHints(dsp_, win_, &hints);
    XMapWindow(dsp_, win_);
    XFlush(dsp_);
}

void Window::dispose() {
    XCloseDisplay(impl_->dsp_);
    impl_->dsp_ = nullptr;
}

void Window::dispatch(const Event &e) {
    for (const auto &listener : listeners_) {
        try {
            listener->listen(e);
        } catch (std::runtime_error& e) {
            fprintf(stderr, "error: %s\n", e.what());
        }
    }
}

void Window::update() {
    XPutImage(impl_->dsp_, impl_->win_, impl_->gc_, impl_->frame_, 0, 0, 0, 0, compositor_->framebuffer().width(), compositor_->framebuffer().height());
}

void Window::setCursor(Window::CursorStyle cs) {
    switch (cs) {
        case CursorStyle::POINTER:
            XDefineCursor(impl_->dsp_, impl_->win_,
                          XCreateFontCursor(impl_->dsp_, XC_hand1));
            break;
        default:
            XDefineCursor(impl_->dsp_, impl_->win_,
                          XCreateFontCursor(impl_->dsp_, XC_arrow));
    }
}

} // jlib