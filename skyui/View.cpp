#include "View.h"
#include "Painter.h"
#include "basic_ds.h"

namespace {

    jlib::Pixmap buildFilletMask(const jlib::Size& sz, size_t radius) {
        jlib::Pixmap mask(sz);
        jlib::Painter painter(mask);
        jlib::Pen pen;
        pen.set_color(0xFFFFFFFF);
        painter.setPen(&pen);
        mask.fill(0x00FFFFFF);
        painter.DrawRect({0, 0}, (int) mask.width(), (int) mask.height(), radius);
        return mask;
    }

}

namespace jlib {

    View::View(const Size& sz) : content_(sz), alpha_(false), position_(), z_(0), fillet_(0) {

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

}