#include "Compositor.h"

namespace jlib {

    Widget *rcMousePressedView = nullptr;

    Widget *rcActiveView = nullptr; // press esc to cancel

    Widget *rcMouseOnView = nullptr;

    Compositor::Compositor(const Size &sz) : framebuffer_(sz), display_(nullptr) {

    }

    void Compositor::flush() {
        framebuffer_.fill(0xFFFFFFFF);
        for (auto &widget: stack_) {
            if (!widget->isVisible()) continue;
            auto shadow = widget->shadow();
            auto pos = shadow->pos();
            widget->onPaint();
//            printf("[Shadow] shadow (%d, %d)\n", pos.x, pos.y);
            framebuffer_.blend(shadow->shadow(), pos);
            if (widget->alpha()) {
                framebuffer_.blend(widget->compose(), widget->pos());
            } else {
                framebuffer_.cover(widget->compose(), widget->pos());
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
            case EventType::WindowResize:
                onResize(EventUtil::asWindowResizeEvent(e));
                break;
            default:;
        }
    }

    Widget *Compositor::locate(const Point &pos) {
        for (int i = (int) stack_.size() - 1; i > -1; i--) {
            auto widget = stack_[i];
            if (widget->contains(pos)) return widget;
        }
        return nullptr;
    }

    void Compositor::onMouseMove(const MouseMoveEvent &e) {
        auto widget = locate(e->pos());
        if (!widget) {
            if (rcMouseOnView) {
                rcMouseOnView->onMouseLeave(e);
                flush();
//                printf("Compositor::onMouseMove rcMouseOnView : %p, widget : %p, onMouseLeave : %p\n", rcMouseOnView, widget, rcMouseOnView);
            }
            rcMouseOnView = nullptr;
        } else {
            if (!rcMouseOnView) {
                widget->onMouseEnter(e);
                flush();
//                printf("Compositor::onMouseMove rcMouseOnView : %p, widget : %p, onMouseEnter : %p\n", rcMouseOnView, widget, widget);
            } else if (rcMouseOnView != widget) {
                rcMouseOnView->onMouseLeave(e);
                widget->onMouseEnter(e);
//                printf("Compositor::onMouseMove rcMouseOnView : %p, widget : %p, onMouseLeave : %p, onMouseEnter : %p\n",
//                       rcMouseOnView, widget, rcMouseOnView, widget);
                flush();
            }
            rcMouseOnView = widget;
        }
    }

    void Compositor::onMousePress(const MousePressEvent &e) {
        auto widget = locate(e->pos());
        if (!widget) return;
        widget->onMousePress(e);
        rcMousePressedView = widget;
        rcActiveView = widget;
        flush();
    }

    void Compositor::onMouseRelease(const MouseReleaseEvent &e) {
        auto widget = locate(e->pos());
        if (!widget) return;
        if (rcMousePressedView == widget) widget->onClick(e);
        widget->onMouseRelease(e);
        rcMousePressedView = nullptr;
        flush();
    }

    void Compositor::onMouseWheel(const MouseWheelEvent &e) {
        auto widget = locate(e->pos());
        if (!widget) return;
        widget->onMouseWheel(e);
    }

    void Compositor::onKeyPress(const KeyPressEvent &e) {
        if (!rcActiveView) return;
        rcActiveView->onKeyPress(e);
    }

    void Compositor::onKeyRelease(const KeyReleaseEvent &e) {
        if (!rcActiveView) return;
        rcActiveView->onKeyRelease(e);
    }

    void Compositor::onResize(const WindowResizeEvent &e) {
        framebuffer_ = Pixmap(e->size());
//        stack_[0]->onResize(e);
    }

}
