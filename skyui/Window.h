#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <algorithm>
#include "Pixmap.h"
#include "Event.h"
#include "Compositor.h"
#include "basic_trait.h"








namespace jlib {

//    class Window {
//    public:
//
//    };









class Window {
public:
    enum CursorStyle { DEFAULT, POINTER };
    Window(Compositor *compositor, int x, int y, const char *title);
    ~Window();
    void run();
    void update();
    void addEventListener(IEventListener *listener)
    { listeners_.emplace_back(listener); }
    void removeEventListener(IEventListener *listener)
    { listeners_.erase(std::find(listeners_.begin(), listeners_.end(), listener)); }
    void setCursor(CursorStyle cs);
private:
    class PrivateWindow;
    using AutoPrivateWindow = std::shared_ptr<PrivateWindow>;
    void create();
    void dispose();
    void dispatch(const Event& e);
    int     x_, y_;
    std::string title_;
    Compositor *compositor_;
//    size_t  width_, height_;
//    xrgb_t *framebuffer_;
    AutoPrivateWindow impl_;
    std::vector<IEventListener*> listeners_;
};

} // jlib

#endif //WINDOW_H
