#ifndef VIEW_H
#define VIEW_H

#include "Pixmap.h"
#include "basic_ds.h"
#include "Event.h"

namespace jlib {

class Widget;

class basic_shadow {
public:
    explicit basic_shadow(Widget& widget);
    basic_shadow(const basic_shadow&) = default;
    void setColor  (xrgb_t color) { color_ = color; }
    void setBlur   (double blur) { blur_ = blur; }
    void setOffsetX(double x) { offsetX_ = x; }
    void setOffsetY(double y) { offsetY_ = y; }
    void setSpread (double spread) { spread_ = spread; }
    void setInset  (bool inset) { inset_ = inset; }
    double offsetX() const { return offsetX_; }
    double offsetY() const { return offsetY_; }
    double    blur() const { return blur_; }
    double  spread() const { return spread_; }
    double   color() const { return color_; }
    bool     inset() const { return inset_; }
    Pixmap build();
    int x() const;
    int y() const;
    Point pos() const;
private:
    Widget& widget_;
    double offsetX_, offsetY_,
           blur_, spread_;
    xrgb_t color_;
    bool   inset_;
};

using Shadow = std::shared_ptr<basic_shadow>;

class Widget : public IEventHandle {
public:
    enum WidgetState { NORMAL, HIDDEN };
    enum WidgetStyle { SOLID = 0x01, TRANSLUCENT = 0x10  };
    Widget(const Widget&) = default;
    explicit Widget(const Size& sz);
    virtual ~Widget() = default;
    const std::string& uuid() const { return uuid_; }
    void move(int x, int y) { position_ = {x, y}; }
    void move(const Point& pos) { position_ = pos; }
    void resize(const Size& sz);
    void setFillet(size_t radius);
    void setShadow(int offset_x = 0, int offset_y = 0,
                   double blur = 5.0, double spread = 1.2,
                   xrgb_t color = 0xFF000000, bool inset = false);
    int x() const { return position_.x; }
    int y() const { return position_.y; }
    int z() const { return z_; }
    size_t  width() const { return content_.width(); }
    size_t height() const { return content_.height(); }
    Pixmap  compose();
    Pixmap& content() { return content_; }
    Point  position() const { return position_; }
    bool      alpha() const { return style_ == TRANSLUCENT; }
    size_t   fillet() const { return fillet_; }
    bool contains(const Point& pos) const;
    void show() { state_ = NORMAL; }
    void hide() { state_ = HIDDEN; }
    bool isVisible() const { return state_ != HIDDEN; }
    Shadow shadow() { return shadow_; }
//    bool hasShadow() const { return shadow_ != nullptr; }
    virtual void paint();
    // Events processing
    void onMouseMove(const MouseMoveEvent& e) override;
    void onMousePress(const MousePressEvent& e) override;
    void onMouseRelease(const MouseReleaseEvent& e) override;
    void onMouseWheel(const MouseWheelEvent& e) override;
    void onKeyPress(const KeyPressEvent& e) override;
    void onKeyRelease(const KeyReleaseEvent & e) override;
    void onClick(const MouseReleaseEvent& e) override;
    void onResize(const jlib::WindowResizeEvent &e) override;
private:
    int         z_;
    Point       position_;
    Pixmap      content_;
    size_t      fillet_;
    std::string uuid_;
    WidgetState state_;
    WidgetStyle style_;
    Shadow      shadow_;
};

}


#endif //VIEW_H
