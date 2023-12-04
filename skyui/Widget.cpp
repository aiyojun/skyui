#include <Magick++/Image.h>
#include "Painter.h"
#include "Widget.h"
#include "utils.h"

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

    jlib::Pixmap generateShadow(jlib::Widget& widget, double spread, double blur, jlib::xrgb_t color) {
        size_t width, height, outbox_width, outbox_height;
        width = widget.width(); height = widget.height();
        jlib::Pixmap pxm = widget.mask();
        if (pxm.empty()) pxm = widget.canvas();
        outbox_width = (size_t) ((double) width + spread * 2);
        outbox_height = (size_t) ((double) height + spread * 2);
        jlib::Pixmap outbox(outbox_width, outbox_height);
        outbox.fill(0x00FFFFFF);
//        Point p0{(int) spread, (int) spread};
//        outbox.fillRect(color, p0, {(int) (p0.x + width()), (int) (p0.y + height())});
        jlib::xrgb_t *ptr_d = outbox.data(), *ptr_s = pxm.data();
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                unsigned int x = (ptr_s[width * j + i] & 0xFF000000) >> 24;
                ptr_d[(int) ((double) outbox_width * (spread + j) + spread + i)]
                        = jlib::ColorUtil::alpha(color, (float) x / 255);
            }
        }
        Magick::Image img;
        img.backgroundColor(Magick::Color(255, 255, 255, 0));
        img.read(outbox_width, outbox_height, "BGRA", Magick::CharPixel, (void *) outbox.data());
        img.blur(spread, blur);
        img.write(0, 0, outbox_width, outbox_height, "BGRA", Magick::CharPixel, (void *) outbox.data());
        return outbox;
    }

}

namespace jlib {

    Widget::Widget(const Size& sz, size_t radius)
    : x_(0), y_(0), z_(0), content_(Pixmap(sz)),
      mask_(radius != 0 ? buildFilletMask(sz, radius) : Pixmap()),
      fillet_(radius), uuid_(generate_uuid()), state_(NORMAL),
      style_(radius != 0 ? TRANSLUCENT : SOLID), shadow_(nullptr) {}

    const std::string &Widget::uuid() const { return /* prv_-> */uuid_; }

    void Widget::move(int x, int y) { /* prv_-> */x_ = x; /* prv_-> */y_ = y; }

    void Widget::move(const Point &pos) { /* prv_-> */x_ = pos.x; /* prv_-> */y_ = pos.y; }

    int Widget::x() const { return /* prv_-> */x_; }

    int Widget::y() const { return /* prv_-> */y_; }

    int Widget::z() const { return /* prv_-> */z_; }

    size_t Widget::width() const { return /* prv_-> */content_.width(); }

    size_t Widget::height() const { return /* prv_-> */content_.height(); }

    Pixmap &Widget::canvas() { return /* prv_-> */content_; }

    const Pixmap &Widget::mask() const { return /* prv_-> */mask_; }

    const Shadow &Widget::shadow() const { return /* prv_-> */shadow_; }

    Point Widget::pos() const { return {/* prv_-> */x_, /* prv_-> */y_}; }

    size_t Widget::fillet() const { return /* prv_-> */fillet_; }

    bool Widget::alpha() const { return /* prv_-> */style_ == TRANSLUCENT; }

    void Widget::show() { /* prv_-> */state_ = NORMAL; }

    void Widget::hide() { /* prv_-> */state_ = HIDDEN; }

    bool Widget::isVisible() const { return /* prv_-> */state_ != HIDDEN; }

    Pixmap Widget::compose() {
        if (/* prv_-> */fillet_) {
            return canvas().mask(/* prv_-> */mask_);
        }
        return canvas();
    }

    bool Widget::contains(const Point &pos) const {
        return pos.x >= x() && pos.x <= x() + width() && pos.y >= y() && pos.y <= y() + height();
    }

    void Widget::onMouseMove(const MouseMoveEvent& e) {}

    void Widget::onMousePress(const MousePressEvent& e) {}

    void Widget::onMouseRelease(const MouseReleaseEvent& e) {}

    void Widget::onMouseWheel(const MouseWheelEvent& e) {}

    void Widget::onKeyPress(const KeyPressEvent& e) {}

    void Widget::onKeyRelease(const KeyReleaseEvent & e) {}

    void Widget::onClick(const MouseReleaseEvent& e) {}

    void Widget::resize(const Size &sz) {
        if (sz != size())
            content_ = Pixmap(sz);
    }

    void Widget::onResize(const WindowResizeEvent &e) {
        resize(e->size());
//        if (e->size() != size())
//            content_ = Pixmap(e->size());
    }

    void Widget::onPaint() {}

    void Widget::decorateShadow(int offset_x, int offset_y, double blur, double spread, xrgb_t color, bool inset) {
        /* prv_-> */shadow_ = std::make_shared<basic_shadow>(*this, spread, blur, color);
        /* prv_-> */shadow_->setOffset(offset_x, offset_y);
    }

    void Widget::onMouseEnter(const MouseEvent &e) {}

    void Widget::onMouseLeave(const MouseEvent &e) {}

    Size Widget::size() const {
        return content_.size();
    }

    void Widget::bind(EventType e, const std::function<void()> &fn) {
        if (listeners_.find(e) == listeners_.end())
            listeners_[e] = {};
        listeners_[e].emplace_back(fn);
    }

    void Widget::activate(EventType e) {
        if (listeners_.find(e) == listeners_.end()) return;
        for (const auto& listener : listeners_[e]) {
            try {
                listener();
            } catch (std::exception& e) {
                fprintf(stderr, "error: %s", e.what());
            }
        }
    }

    basic_shadow::basic_shadow(Widget& widget, double spread, double blur, xrgb_t color)
            : widget_(widget), offsetX_(0), offsetY_(0), inset_(false),
              spread_(spread), blur_(blur), color_(color),
              shadow_(generateShadow(widget, spread, blur, color)) {

    }

    int basic_shadow::x() const {
        return (int) ((double) widget_.x() - spread_ + offsetX_);
    }

    int basic_shadow::y() const {
        return (int) ((double) widget_.y() - spread_ + offsetY_);
    }

    Point basic_shadow::pos() const {
        return {(int) ((double) widget_.x() - spread_ + offsetX_), (int) ((double) widget_.y() - spread_ + offsetY_)};
    }

}