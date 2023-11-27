#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "Pixmap.h"

using XWindow = ::Window;

namespace jlib {

class Window {
public:
    Window(Pixmap *pixmap, int x, int y, const char *title);
    ~Window();
    void Run();
private:
    void Create();
    void Dispose();
    int width_, height_, x_, y_;
    std::string title_;
    Display*    dsp_;
    XWindow     win_;
    XWindow     root_;
    GC          gc_;
    XImage*     frame_;
    XVisualInfo vsInfo_;
    XSetWindowAttributes win_attrs_;
    xrgb_t*       fb_;
};

} // jlib

#endif //WINDOW_H
