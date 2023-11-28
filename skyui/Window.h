#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <algorithm>
#include "Pixmap.h"
#include "Event.h"
#include "Compositor.h"

namespace jlib {

class Window {
public:
    Window(const Pixmap& pixmap, int x, int y, const char *title);
    ~Window();
    void run();
    void update();
    void addEventListener(EventListener *listener)
    { listeners_.emplace_back(listener); }
    void removeEventListener(EventListener *listener)
    { listeners_.erase(std::find(listeners_.begin(), listeners_.end(), listener)); }
private:
    class PrivateWindow;
    using AutoPrivateWindow = std::shared_ptr<PrivateWindow>;
    void create();
    void dispose();
    void dispatch(const Event& e);
    int     x_, y_;
    std::string title_;
    size_t  width_, height_;
    xrgb_t *framebuffer_;
    AutoPrivateWindow impl_;
    std::vector<EventListener*> listeners_;
};

} // jlib

#endif //WINDOW_H
