#ifndef COMPOSITOR_H
#define COMPOSITOR_H


#include <map>
#include "View.h"
#include "Pixmap.h"
#include "Event.h"

namespace jlib {

class Compositor {
public:
    explicit Compositor(const Size& sz);
    Compositor(const Compositor&) = default;
    void flush();
    void push(const View& v) { stack_.emplace_back(v); }
    Pixmap& framebuffer() { return framebuffer_; }
    void dispatch(const Event& e);
private:
    std::vector<View> stack_;
    Pixmap framebuffer_;
};

};

#endif //COMPOSITOR_H
