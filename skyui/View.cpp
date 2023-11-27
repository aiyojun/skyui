#include "View.h"
#include "Painter.h"

namespace jlib {

View::View(int w, int h) : content_(w, h), mask_(w, h), x_(0), y_(0), z_(0), fillet_(0) {

}

void View::Prepare() {

}

void View::set_background_color(xrgb_t c) {
    background_color_ = c;
    content_.Fill(background_color_);
}

void View::set_position(int x, int y) {
    x_ = x;
    y_ = y;
}

void View::set_fillet(int f) {
    fillet_ = f;
    alpha_ = true;
    Painter painter(&mask_);
    Pen pen;
    pen.set_color(0xFFFFFFFF);
    painter.set_pen(&pen);
    mask_.Fill(0x00FFFFFF);
    painter.DrawRect({0, 0}, mask_.width(), mask_.height(), fillet_);
}


}