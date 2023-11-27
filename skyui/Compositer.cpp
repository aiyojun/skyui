#include "Compositor.h"
#include <cstring>
#include <stdexcept>

namespace jlib {

Compositor::Compositor(int w, int h) : framebuffer_(new Pixmap(w, h)) {

}

Compositor::~Compositor() {
    clean();
}

void Compositor::flush() {
    framebuffer_->Fill(0xFFFFFFFF);
    for (auto & view : stack_) {
        if (view->fillet()) {
            Pixmap::Blend(view->content(), view->mask(), {0, 0});
        }
        framebuffer_->Overlay(view->content(), view->position(), view->alpha());
    }
}

void Compositor::clean() {
    if (framebuffer_) {
        delete framebuffer_;
        framebuffer_ = nullptr;
    }
}


}
