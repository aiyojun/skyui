#include <random>
#include <stdexcept>
#include <Magick++/Image.h>
#include "Painter.h"
#include "Widget.h"

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

    jlib::Pixmap generateShadow(jlib::Widget& widget, double spread, double blur, jlib::xrgb_t color) {
        printf("shadow parameters => blur : %f, spread : %f\n", blur, spread);
        size_t width, height, outbox_width, outbox_height;
        width = widget.width(); height = widget.height();
        jlib::Pixmap pxm = widget.mask();
        if (pxm.empty()) {
            printf("shadow use content, not mask\n");
            pxm = widget.content();
        } else {
            printf("shadow use mask\n");
        }
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

    class Widget::PrivateWidget {
    public:
        PrivateWidget(const Size& sz, size_t radius);
        int         x_, y_, z_;
        Pixmap      content_, mask_;
        size_t      fillet_;
        std::string uuid_;
        WidgetState state_;
        WidgetStyle style_;
        Shadow      shadow_;
    };

    Widget::PrivateWidget::PrivateWidget(const Size& sz, size_t radius)
    : x_(0), y_(0), z_(0), content_(Pixmap(sz)),
      mask_(radius != 0 ? buildFilletMask(sz, radius) : Pixmap()),
      fillet_(radius), uuid_(generate_uuid()), state_(NORMAL),
      style_(radius != 0 ? TRANSLUCENT : SOLID), shadow_(nullptr) {

    }

    Widget::Widget(const Size &sz, size_t radius)
            : prv_(std::make_shared<PrivateWidget>(sz, radius)) {}

    const std::string &Widget::uuid() const { return prv_->uuid_; }

    void Widget::move(int x, int y) {
        prv_->x_ = x;
        prv_->y_ = y;
    }

    void Widget::move(const Point &pos) {
        prv_->x_ = pos.x;
        prv_->y_ = pos.y;
    }

    int Widget::x() const { return prv_->x_; }

    int Widget::y() const { return prv_->y_; }

    int Widget::z() const { return prv_->z_; }

    size_t Widget::width() const { return prv_->content_.width(); }

    size_t Widget::height() const { return prv_->content_.height(); }

    Pixmap &Widget::content() { return prv_->content_; }

    const Pixmap &Widget::mask() const { return prv_->mask_; }

    const Shadow &Widget::shadow() const { return prv_->shadow_; }

    Point Widget::position() const { return {prv_->x_, prv_->y_}; }

    size_t Widget::fillet() const { return prv_->fillet_; }

    bool Widget::alpha() const { return prv_->style_ == TRANSLUCENT; }

    void Widget::show() { prv_->state_ = NORMAL; }

    void Widget::hide() { prv_->state_ = HIDDEN; }

    bool Widget::isVisible() const { return prv_->state_ != HIDDEN; }

    Pixmap Widget::compose() {
        if (prv_->fillet_) {
            return content().mask(prv_->mask_);
        }
        return content();
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

    void Widget::decorateShadow(int offset_x, int offset_y, double blur, double spread, xrgb_t color, bool inset) {
        prv_->shadow_ = std::make_shared<basic_shadow>(*this, spread, blur, color);
        prv_->shadow_->setOffset(offset_x, offset_y);
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
        printf("shadow get x, widget : %s (%d, %d), spread : %d\n", widget_.uuid().c_str(), widget_.x(), widget_.y(), (int) spread_);
        return {(int) ((double) widget_.x() - spread_ + offsetX_), (int) ((double) widget_.y() - spread_ + offsetY_)};
    }

}