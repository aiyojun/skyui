#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <map>
#include <vector>
#include "View.h"
#include "Pixmap.h"
#include "Event.h"
#include "basic_trait.h"

namespace jlib {

class Compositor : public EventHandle {
public:
    explicit Compositor(const Size& sz);
    Compositor(const Compositor&) = default;
    void flush();
    void push(const View& v) { stack_.emplace_back(v); }
    Pixmap& framebuffer() { return framebuffer_; }
    void dispatch(const Event& e);
    void setDisplay(IBuffer *p) { display_ = p; }
    View* locate(const Point& pos);

    void onMouseMove(const MouseMoveEvent& e) override;
    void onMousePress(const MousePressEvent& e) override;
    void onMouseRelease(const MouseReleaseEvent& e) override;
    void onMouseWheel(const MouseWheelEvent& e) override;
    void onKeyPress(const KeyPressEvent& e) override;
    void onKeyRelease(const KeyReleaseEvent & e) override;
    void onClick(const MouseReleaseEvent& e) override;

    std::vector<View> stack_;
    Pixmap framebuffer_;
    IBuffer *display_;
};

};

#endif //COMPOSITOR_H
