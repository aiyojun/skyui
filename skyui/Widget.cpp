#include <random>
#include "Widget.h"
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

    std::string generate_uuid(int n = 6) {
        std::string uuid;
        static std::default_random_engine e(time(nullptr));
        static std::uniform_int_distribution<int> u(0,15);
        char mapper[] = {
                '0', '1', '2', '3', '4', '5', '6', '7',
                '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        for (int i = 0; i < n; i++) {
            uuid += mapper[u(e)];
        }
        return uuid;
    }

}

namespace jlib {

    Widget::Widget(const Size& sz)
    : uuid_(generate_uuid()), content_(sz),
      position_(), z_(0), fillet_(0),
      state_(NORMAL), style_(SOLID) {

    }

    Pixmap Widget::compose() {
        if (fillet_) {
            return content().mask(buildFilletMask(content().size(), fillet_));
        }
        return content();
    }

    void Widget::setFillet(size_t radius)  {
        if (!radius) return;
        fillet_ = radius;
        style_  = TRANSLUCENT;
//        alpha_ = true;
    }

    bool Widget::contains(const Point &pos) const {
        return pos.x >= x() && pos.x <= x() + width() && pos.y >= y() && pos.y <= y() + height();
    }

    void Widget::onMouseMove(const MouseMoveEvent& e) {
        printf("[onMouseMove] \n");
    }

    void Widget::onMousePress(const MousePressEvent& e) {
        printf("[onMousePress] \n");
    }

    void Widget::onMouseRelease(const MouseReleaseEvent& e) {
        printf("[onMouseRelease] \n");
    }

    void Widget::onMouseWheel(const MouseWheelEvent& e) {
        printf("[onMouseWheel] \n");
    }

    void Widget::onKeyPress(const KeyPressEvent& e) {
        printf("[onKeyPress] \n");
    }

    void Widget::onKeyRelease(const KeyReleaseEvent & e) {
        printf("[onKeyRelease] \n");
    }

    void Widget::onClick(const MouseReleaseEvent& e) {
        printf("[onClick] view %s\n", uuid().c_str());
    }

    void Widget::resize(const Size &sz) {

    }

    void Widget::onResize(const WindowResizeEvent &e) {

    }

    void Widget::paint() {

    }

    void Widget::setShadow(int offset_x, int offset_y, double blur, double spread, xrgb_t color, bool inset) {
        shadow_ = std::make_shared<basic_shadow>(*this);
        shadow_->setOffsetX(offset_x);
        shadow_->setOffsetY(offset_y);
        shadow_->setBlur(blur);
        shadow_->setSpread(spread);
        shadow_->setColor(color);
        shadow_->setInset(inset);
    }

    int basic_shadow::x() const {
        return (int) ((double) widget_.x() - (double) widget_.width() * (spread_ - 1.0) * 0.5 + offsetX_);
    }

    int basic_shadow::y() const {
        return (int) ((double) widget_.y() - (double) widget_.height() * (spread_ - 1.0) * 0.5 + offsetY_);
    }

    Pixmap basic_shadow::build() {
        return widget_.content().shadow(blur_, spread_, color_, inset_);
    }

    Point basic_shadow::pos() const {
        return {x(), y()};
    }

    basic_shadow::basic_shadow(Widget &widget)
    : widget_(widget), offsetX_(0), offsetY_(0),
    blur_(5), spread_(1.2), color_(0xFF000000), inset_(false) {}
}