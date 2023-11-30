#ifndef WIDGET_H
#define WIDGET_H

#include "basic_ds.h"
#include "Pixmap.h"
#include "Event.h"

namespace jlib {

class basic_shadow;

using Shadow = std::shared_ptr<basic_shadow>;

class Widget : public IEventHandle {
public:
    enum WidgetState { NORMAL, HIDDEN };
    enum WidgetStyle { SOLID = 0x01, TRANSLUCENT = 0x10  };
    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;
    explicit Widget(const Size& sz, size_t radius = 0);
    virtual ~Widget() = default;
    const std::string& uuid() const;
    void show();
    void hide();
    void move(int x, int y);
    void move(const Point& pos);
    void resize(const Size& sz);
    void decorateShadow(int offset_x = 0, int offset_y = 0,
                        double blur = 3.0, double spread = 5.0,
                        xrgb_t color = 0x88000000, bool inset = false);
    int x() const;
    int y() const;
    int z() const;
    size_t  width() const;
    size_t height() const;
    Pixmap  compose();
    Pixmap& content();
//    const Pixmap&   area() const;
    const Pixmap&   mask() const;
    const Shadow& shadow() const;
    Point       position() const;
    size_t        fillet() const;
    bool           alpha() const;
    bool contains(const Point& pos) const;
    bool isVisible() const;
    virtual void paint();
    // Events processing
    void onMouseMove   (const MouseMoveEvent    &e) override;
    void onMousePress  (const MousePressEvent   &e) override;
    void onMouseRelease(const MouseReleaseEvent &e) override;
    void onMouseWheel  (const MouseWheelEvent   &e) override;
    void onKeyPress    (const KeyPressEvent     &e) override;
    void onKeyRelease  (const KeyReleaseEvent   &e) override;
    void onClick       (const MouseReleaseEvent &e) override;
    void onResize      (const WindowResizeEvent &e) override;
private:
    class PrivateWidget;
    using AutoPrivateWidget = std::shared_ptr<PrivateWidget>;
    AutoPrivateWidget prv_;
};

class basic_shadow {
public:
    explicit basic_shadow(Widget& widget, double spread, double blur, xrgb_t color);
    basic_shadow(const basic_shadow&) = default;
    void setOffsetX(double x) { offsetX_ = x; }
    void setOffsetY(double y) { offsetY_ = y; }
    void setOffset(double x, double y) { offsetX_ = x; offsetY_ = y; }
    double offsetX() const { return offsetX_; }
    double offsetY() const { return offsetY_; }
    double    blur() const { return blur_; }
    double  spread() const { return spread_; }
    double   color() const { return color_; }
    bool     inset() const { return inset_; }
    Pixmap  shadow() const { return shadow_; }
    int          x() const;
    int          y() const;
    Point      pos() const;
private:
    Widget  widget_;
    Pixmap  shadow_;
    double  offsetX_, offsetY_,
            blur_, spread_;
    xrgb_t  color_;
    bool    inset_;
};

}


#endif //WIDGET_H
