#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <map>
#include <vector>
#include "Widget.h"
#include "Pixmap.h"
#include "Event.h"
#include "basic_trait.h"

namespace jlib {

class Compositor : public IEventHandle {
public:
    explicit Compositor(const Size& sz);
    Compositor(const Compositor&) = default;
    void flush();
    void push(const Widget& v) { stack_.emplace_back(v); }
    Pixmap& framebuffer() { return framebuffer_; }
    void dispatch(const Event& e);
    void setDisplay(IBuffer *p) { display_ = p; }
    Widget* locate(const Point& pos);
    // events agent
    void onMouseMove(const MouseMoveEvent& e) override;
    void onMousePress(const MousePressEvent& e) override;
    void onMouseRelease(const MouseReleaseEvent& e) override;
    void onMouseWheel(const MouseWheelEvent& e) override;
    void onKeyPress(const KeyPressEvent& e) override;
    void onKeyRelease(const KeyReleaseEvent & e) override;
    void onClick(const MouseReleaseEvent& e) override;
    void onResize(const jlib::WindowResizeEvent &e) override;

private:
    std::vector<Widget> stack_;
    Pixmap framebuffer_;
    IBuffer *display_;
};

};

#endif //COMPOSITOR_H
