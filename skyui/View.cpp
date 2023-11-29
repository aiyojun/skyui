#include <random>
#include "View.h"
#include "Painter.h"
#include "basic_ds.h"

namespace {

    jlib::Pixmap buildFilletMask(const jlib::Size& sz, size_t radius) {
        jlib::Pixmap mask(sz);
        jlib::Painter painter(mask);
        jlib::Pen pen;
        pen.setColor(0xFFFFFFFF);
        painter.setPen(&pen);
        mask.fill(0x00FFFFFF);
        painter.drawRect({0, 0}, (int) mask.width(), (int) mask.height(), radius);
        return mask;
    }

    std::string generate_uuid(int n = 12) {
        std::string uuid;
        static std::default_random_engine e(time(nullptr));
        static std::uniform_int_distribution<int> u(0,15);
        char mapper[] = {
                '0', '1', '2', '3', '4', '5', '6', '7',
                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        for (int i = 0; i < n; i++) {
            uuid += mapper[u(e)];
        }
        printf("uuid : %s\n", uuid.c_str());
        return uuid;
    }

}

namespace jlib {

    View::View(const Size& sz) : uuid_(generate_uuid()), content_(sz), alpha_(false), position_(), z_(0), fillet_(0) {

    }

    Pixmap View::compose() {
        if (fillet_) {
            return content().mask(buildFilletMask(content().size(), fillet_));
        }
        return content();
    }

    void View::setFillet(size_t radius)  {
        if (!radius) return;
        fillet_ = radius;
        alpha_ = true;
    }

    bool View::contains(const Point &pos) const {
        return pos.x >= x() && pos.x <= x() + width() && pos.y >= y() && pos.y <= y() + height();
    }

    void View::onMouseMove(const MouseMoveEvent& e) {
        printf("[onMouseMove] \n");
    }

    void View::onMousePress(const MousePressEvent& e) {
        printf("[onMousePress] \n");
    }

    void View::onMouseRelease(const MouseReleaseEvent& e) {
        printf("[onMouseRelease] \n");
    }

    void View::onMouseWheel(const MouseWheelEvent& e) {
        printf("[onMouseWheel] \n");
    }

    void View::onKeyPress(const KeyPressEvent& e) {
        printf("[onKeyPress] \n");
    }

    void View::onKeyRelease(const KeyReleaseEvent & e) {
        printf("[onKeyRelease] \n");
    }

    void View::onClick(const MouseReleaseEvent& e) {
        printf("[onClick] view %s\n", uuid().c_str());
    }

}