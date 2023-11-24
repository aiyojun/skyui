#ifndef GWF_WIDGET_H
#define GWF_WIDGET_H

#include <string>

namespace jlib {

class Event {};

class MouseEvent : public Event {
public:
    int x() const { return x_; }
    int y() const { return y_; }
private:
    int x_, y_;
};

class Widget {
public:
    enum WidgetStyle { SOLID, TRANSLUCENT, ALPHA };
    enum WidgetState { NORMAL, HIDDEN };
    void setGeometry() {}
    void setSize() {}
    void setPosition() {}
    void show() {}
    void hide() {}
    virtual void onClick(MouseEvent e);
private:
    std::string id_;
    int x_, y_, width_, height_;
    WidgetStyle style_;
    WidgetState state_;
    void *view_;
};

} // jlib

#endif //GWF_WIDGET_H
