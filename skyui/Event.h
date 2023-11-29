#ifndef EVENT_H
#define EVENT_H

#include <set>
#include <memory>
#include "basic_ds.h"

namespace jlib {

    enum EventType {
        MouseWheel, MouseMove, MousePress,
        MouseRelease, KeyPress, KeyRelease
    };

    class basic_event {
    public:
        virtual ~basic_event() = default;
        EventType type() const { return type_; }
    protected:
        explicit basic_event(EventType t) : type_(t) {}
    private:
        EventType type_;
    };

    enum MouseButton { LEFT, RIGHT, MIDDLE };

    class Mouse {
    public:
        static const Mouse& getInstance();
        Mouse(const Mouse&) = delete;
        void press  (MouseButton button) { buttons_.insert(button); }
        void release(MouseButton button) { buttons_.erase(button); }
        bool pressed(MouseButton button) const
        { return buttons_.find(button) != buttons_.end(); }
    private:
        Mouse() = default;
        static Mouse *instance_;
        std::set<MouseButton> buttons_;
    };

    class Keyboard {
    public:
        static const Keyboard& getInstance();
        Keyboard(const Keyboard&) = delete;
        void press(size_t key) { keys_.insert(key); }
        void release(size_t key) { keys_.erase(key); }
        bool shift() { return keys_.find(66) != keys_.end(); }
        bool  ctrl() { return keys_.find(37) != keys_.end(); }
        bool   alt() { return keys_.find(64) != keys_.end(); }
    private:
        Keyboard() = default;
        static Keyboard *instance_;
        std::set<int> keys_;
    };

    class basic_mouse_event : public basic_event {
    protected:
        basic_mouse_event(EventType t, int x, int y) : basic_event(t), x_(x), y_(y) {}
    public:
        ~basic_mouse_event() override = default;
        int x() const { return x_; }
        int y() const { return y_; }
        Point pos() const { return {x_, y_}; }
    private:
        int x_, y_;
    };

    class basic_mouse_move_event : public basic_mouse_event {
    public:
        basic_mouse_move_event(int x, int y) : basic_mouse_event(MouseMove, x, y) {}
        ~basic_mouse_move_event() override = default;
    };

    class basic_mouse_press_event : public basic_mouse_event {
    public:
        basic_mouse_press_event(int x, int y, MouseButton bt) : basic_mouse_event(MousePress, x, y), button_(bt) {}
        ~basic_mouse_press_event() override = default;
        MouseButton button() const { return button_; }
    private:
        MouseButton button_;
    };

    class basic_mouse_release_event : public basic_mouse_event {
    public:
        basic_mouse_release_event(int x, int y, MouseButton bt) : basic_mouse_event(MouseRelease, x, y), button_(bt) {}
        ~basic_mouse_release_event() override = default;
        MouseButton button() const { return button_; }
    private:
        MouseButton button_;
    };

    class basic_mouse_wheel_event : public basic_mouse_event {
    public:
        basic_mouse_wheel_event(int x, int y, int delta) : basic_mouse_event(MouseWheel, x, y), delta_(delta) {}
        ~basic_mouse_wheel_event() override = default;
        int delta() const { return delta_; }
    private:
        int delta_;
    };

    class basic_key_press_event : public basic_event {
    public:
        explicit basic_key_press_event(size_t key) : basic_event(KeyPress), key_(key) {}
        ~basic_key_press_event() override = default;
        size_t key() const { return key_; }
    private:
        size_t key_;
    };

    class basic_key_release_event : public basic_event {
    public:
        explicit basic_key_release_event(size_t key) : basic_event(KeyRelease), key_(key) {}
        ~basic_key_release_event() override = default;
        size_t key() const { return key_; }
    private:
        size_t key_;
    };

    using Event             = std::shared_ptr<basic_event>;
    using MouseEvent        = std::shared_ptr<basic_mouse_event>;
    using MouseMoveEvent    = std::shared_ptr<basic_mouse_move_event>;
    using MousePressEvent   = std::shared_ptr<basic_mouse_press_event>;
    using MouseReleaseEvent = std::shared_ptr<basic_mouse_release_event>;
    using MouseWheelEvent   = std::shared_ptr<basic_mouse_wheel_event>;
    using KeyPressEvent     = std::shared_ptr<basic_key_press_event>;
    using KeyReleaseEvent   = std::shared_ptr<basic_key_release_event>;

    class EventUtil {
    public:
        static MouseMoveEvent buildMouseMoveEvent(int x, int y)
        { return std::make_shared<basic_mouse_move_event>(x, y); }
        static MousePressEvent buildMousePressEvent(int x, int y, MouseButton bt)
        { return std::make_shared<basic_mouse_press_event>(x, y, bt); }
        static MouseReleaseEvent buildMouseReleaseEvent(int x, int y, MouseButton bt)
        { return std::make_shared<basic_mouse_release_event>(x, y, bt); }
        static MouseWheelEvent buildMouseWheelEvent(int x, int y, int delta)
        { return std::make_shared<basic_mouse_wheel_event>(x, y, delta); }
        static KeyPressEvent buildKeyPressEvent(size_t key)
        { return std::make_shared<basic_key_press_event>(key); }
        static KeyReleaseEvent buildKeyReleaseEvent(size_t key)
        { return std::make_shared<basic_key_release_event>(key); }
        static bool isMouseEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_event>(e) != nullptr; }
        static bool isMouseMoveEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_move_event>(e) != nullptr; }
        static bool isMousePressEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_press_event>(e) != nullptr; }
        static bool isMouseReleaseEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_release_event>(e) != nullptr; }
        static bool isMouseWheelEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_wheel_event>(e) != nullptr; }
        static bool isKeyPressEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_key_press_event>(e) != nullptr; }
        static bool isKeyReleaseEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_key_release_event>(e) != nullptr; }
        static MouseEvent asMouseEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_event>(e); }
        static MouseMoveEvent asMouseMoveEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_move_event>(e); }
        static MousePressEvent asMousePressEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_press_event>(e); }
        static MouseReleaseEvent asMouseReleaseEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_release_event>(e); }
        static MouseWheelEvent asMouseWheelEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_mouse_wheel_event>(e); }
        static KeyPressEvent asKeyPressEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_key_press_event>(e); }
        static KeyReleaseEvent asKeyReleaseEvent(const Event& e)
        { return std::dynamic_pointer_cast<basic_key_release_event>(e); }
    };

    class EventListener {
    public:
        virtual void listen(const Event& e) = 0;
    };

    class EventHandle {
    public:
        virtual void onMouseMove(const MouseMoveEvent& e) = 0;
        virtual void onMousePress(const MousePressEvent& e) = 0;
        virtual void onMouseRelease(const MouseReleaseEvent& e) = 0;
        virtual void onClick(const MouseReleaseEvent& e) = 0;
        virtual void onMouseWheel(const MouseWheelEvent& e) = 0;
        virtual void onKeyPress(const KeyPressEvent& e) = 0;
        virtual void onKeyRelease(const KeyReleaseEvent & e) = 0;
    };

} // jlib


#endif //EVENT_H
