#include "Compositor.h"
#include <cstring>
#include <stdexcept>

namespace jlib {

    Compositor::Compositor(const Size &sz) : framebuffer_(sz), display_(nullptr) {

    }

    void Compositor::flush() {
        framebuffer_.fill(0xFFFFFFFF);
        for (auto &view: stack_) {
            if (view.alpha()) {
                auto pxm = view.compose();
                framebuffer_.blend(pxm, view.position());
            } else {
                framebuffer_.cover(view.compose(), view.position());
            }
        }
        if (display_) display_->flush();
    }

    void Compositor::dispatch(const Event &e) {
        switch (e->type()) {
            case EventType::MouseMove:
                onMouseMove(EventUtil::asMouseMoveEvent(e));
                break;
            case EventType::MousePress:
                onMousePress(EventUtil::asMousePressEvent(e));
                break;
            case EventType::MouseRelease:
                onMouseRelease(EventUtil::asMouseReleaseEvent(e));
                break;
            case EventType::MouseWheel:
                onMouseWheel(EventUtil::asMouseWheelEvent(e));
                break;
            case EventType::KeyPress:
                onKeyPress(EventUtil::asKeyPressEvent(e));
                break;
            case EventType::KeyRelease:
                onKeyRelease(EventUtil::asKeyReleaseEvent(e));
                break;
            default:;
        }
    }

    View *Compositor::locate(const Point &pos) {
        for (int i = (int) stack_.size() - 1; i > -1; i--) {
            auto& view = stack_[i];
            if (view.contains(pos)) return &view;
        }
        return nullptr;
    }

    void Compositor::onMouseMove(const MouseMoveEvent &e) {

    }

    View *rcMousePressedView = nullptr;
    View *rcActiveView = nullptr; // press esc to cancel

    void Compositor::onMousePress(const MousePressEvent &e) {
        auto view = locate(e->pos());
        if (!view) return;
        view->onMousePress(e);
        rcMousePressedView = view;
        rcActiveView = view;
    }

    void Compositor::onMouseRelease(const MouseReleaseEvent &e) {
        auto view = locate(e->pos());
        if (!view) return;
        if (rcMousePressedView == view) view->onClick(e);
        view->onMouseRelease(e);
        rcMousePressedView = nullptr;
    }

    void Compositor::onMouseWheel(const MouseWheelEvent &e) {
        auto view = locate(e->pos());
        if (!view) return;
        view->onMouseWheel(e);
    }

    void Compositor::onKeyPress(const KeyPressEvent &e) {
        if (!rcActiveView) return;
        rcActiveView->onKeyPress(e);
    }

    void Compositor::onKeyRelease(const KeyReleaseEvent &e) {
        if (!rcActiveView) return;
        rcActiveView->onKeyRelease(e);
    }

    void Compositor::onClick(const MouseReleaseEvent &e) {

    }

}
