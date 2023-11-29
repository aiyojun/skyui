#include "Window.h"
#include <stdexcept>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

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

Window::Window(const Pixmap& pixmap, int x, int y, const char* title)
    : width_(pixmap.width()), height_(pixmap.height()), x_(x), y_(y), title_(title), framebuffer_(pixmap.data()) {
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
            case MotionNotify: {
//                printf("- move frame %p\n", framebuffer_);
                dispatch(EventUtil::buildMouseMoveEvent(event.xmotion.x, event.xmotion.y));
//                frame_ = XCreateImage(dsp_, info_.visual, info_.depth,
//                                      ZPixmap, 0, reinterpret_cast<char *>(framebuffer_), width_, height_, 8, width_ * 4);
//                XPutImage(impl_->dsp_, impl_->win_, impl_->gc_, impl_->frame_, 0, 0, 0, 0, width_, height_);
//                update();
                break;
            }
            case KeyPress: {
//                char key_buffer[10];
//                KeySym keysym;
//                XLookupString(&event.xkey, key_buffer, 10, &keysym, NULL);
//                printf("Key released: %d - %s\n", event.xkey.keycode, key_buffer);
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
//                else {
//                    printf("ButtonPress %d\n", event.xbutton.button);
//                }
                break;
            }
            case ButtonRelease: {
                if (event.xbutton.button < 4)
                    dispatch(EventUtil::buildMouseReleaseEvent(event.xbutton.x, event.xbutton.y, mapMouseButton(event.xbutton.button)));
//                else {
//                    printf("ButtonRelease %d\n", event.xbutton.button);
//                }
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
    win_ = XCreateWindow(dsp_, root_, x_, y_, width_, height_, 0, info_.depth,
                            InputOutput, info_.visual, CWBackPixel | CWColormap | CWBorderPixel, &attrs_);
    XStoreName(dsp_, win_, title_.c_str());
    XSelectInput(dsp_, win_, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask);
//    XSelectInput(dsp_, win_, ExposureMask | KeyPressMask | PointerMotionMask);
    XGCValues values;
    values.graphics_exposures = 0;
    gc_ = XCreateGC(dsp_, root_, GCGraphicsExposures, &values);
    if (!gc_) throw std::runtime_error("error: fail to create gc.");
    frame_ = XCreateImage(dsp_, info_.visual, info_.depth,
                          ZPixmap, 0, reinterpret_cast<char *>(framebuffer_), width_, height_, 8, width_ * 4);
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
    XPutImage(impl_->dsp_, impl_->win_, impl_->gc_, impl_->frame_, 0, 0, 0, 0, width_, height_);
}

} // jlib