#include "Window.h"
#include <stdexcept>

namespace jlib {

// Window::Window(int width, int height, int x, int y, const char *title)
    // : width_(width), height_(height), x_(x), y_(y), title_(title) {
    // Create();
// }

Window::Window(Pixmap* bitmap, int x, int y, const char* title)
    : width_(bitmap->width()), height_(bitmap->height()), x_(x), y_(y), title_(title), fb_(bitmap->data()) {
    Create();
}

Window::~Window() {
    Dispose();
}

void Window::Run() {
    XEvent event;
    while (!XNextEvent(dsp_, &event)) {
        switch (event.type) {
            case Expose: {
                XPutImage(dsp_, win_, gc_, frame_, 0, 0, 0, 0, width_, height_);
                break;
            }
            case MotionNotify: {
                XPutImage(dsp_, win_, gc_, frame_, 0, 0, 0, 0, width_, height_);
                break;
            }
        }
    }
}

void Window::Create() {
    dsp_ = XOpenDisplay(NULL);
    if (!dsp_)
        throw std::runtime_error("Fail to connect x window.");
    if (!XMatchVisualInfo(dsp_, XDefaultScreen(dsp_), 24, TrueColor, &vsInfo_))
        throw std::runtime_error("No such visual\n");
    root_ = XDefaultRootWindow(dsp_);
    win_attrs_.colormap = XCreateColormap(dsp_, root_, vsInfo_.visual, AllocNone);
    win_attrs_.background_pixel = 0;
    win_attrs_.border_pixel = 0;
    win_ = XCreateWindow(dsp_, root_, x_, y_, width_, height_, 0, vsInfo_.depth,
                            InputOutput, vsInfo_.visual, CWBackPixel | CWColormap | CWBorderPixel, &win_attrs_);
    XStoreName(dsp_, win_, title_.c_str());
    XSelectInput(dsp_, win_, ExposureMask | KeyPressMask | PointerMotionMask);
    XGCValues values;
    values.graphics_exposures = 0;
    gc_ = XCreateGC(dsp_, root_, GCGraphicsExposures, &values);
    if (!gc_) throw std::runtime_error("error: fail to create gc.");
    frame_ = XCreateImage(dsp_, vsInfo_.visual, vsInfo_.depth,
                          ZPixmap, 0, reinterpret_cast<char *>(fb_), width_, height_, 8, width_ * 4);
    XMapWindow(dsp_, win_);
    XFlush(dsp_);
}

void Window::Dispose() {
    XCloseDisplay(dsp_);
}
} // jlib