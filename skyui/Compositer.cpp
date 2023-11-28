#include "Compositor.h"
#include <cstring>
#include <stdexcept>

namespace jlib {

    Compositor::Compositor(const Size &sz) : framebuffer_(sz) {

    }

    void Compositor::flush() {
        framebuffer_.fill(0xFFFFFFFF);
        for (auto &view: stack_) {
            if (view.alpha()) {
                auto pxm = view.compose();
                framebuffer_.blend(pxm, view.position());
            } else {
                framebuffer_.overlay(view.compose(), view.position());
            }
        }
    }

    void Compositor::dispatch(const Event &e) {
        if (EventUtil::isMouseMoveEvent(e)) {
            auto ep = std::dynamic_pointer_cast<basic_mouse_move_event>(e);
//            printf("[MouseMove] (%d, %d)\n", ep->x(), ep->y());
            return;
        }

        if (EventUtil::isMouseWheelEvent(e)) {
            auto ep = std::dynamic_pointer_cast<basic_mouse_wheel_event>(e);
            printf("[MouseWheel] (%d, %d, %d)\n", ep->x(), ep->y(), ep->delta());
            return;
        }

        if (EventUtil::isMousePressEvent(e)) {
            auto ep = std::dynamic_pointer_cast<basic_mouse_press_event>(e);
            printf("[MousePress] (%d, %d, %d)\n", ep->x(), ep->y(), ep->button());
            return;
        }

        if (EventUtil::isMouseReleaseEvent(e)) {
            auto ep = std::dynamic_pointer_cast<basic_mouse_release_event>(e);
            printf("[MouseRelease] (%d, %d, %d)\n", ep->x(), ep->y(), ep->button());
            return;
        }

        if (EventUtil::isKeyPressEvent(e)) {
            auto ep = std::dynamic_pointer_cast<basic_key_press_event>(e);
            printf("[KeyPress] key : %u\n", ep->key());
            return;
        }

        if (EventUtil::isKeyReleaseEvent(e)) {
            auto ep = std::dynamic_pointer_cast<basic_key_release_event>(e);
            printf("[KeyRelease] key : %u\n", ep->key());
            return;
        }
    }


}
