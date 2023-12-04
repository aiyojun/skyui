#ifndef WIDGET_H
#define WIDGET_H

#include <functional>
#include <map>
#include "basic_ds.h"
#include "Pixmap.h"
#include "Event.h"

namespace jlib {

class basic_shadow;

using Shadow = std::shared_ptr<basic_shadow>;

class Widget {
public:
    enum WidgetState { NORMAL, HIDDEN };
    enum WidgetStyle { SOLID = 0x01, TRANSLUCENT = 0x10  };
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    explicit Widget(const Size& sz, size_t radius = 0);
    virtual ~Widget() = default;
    void                     show();
    void                     hide();
    void         move(int x, int y);
    void     move(const Point& pos);
    void     resize(const Size& sz);
    const std::string&       uuid() const;
    int                         x() const;
    int                         y() const;
    int                         z() const;
    size_t                  width() const;
    size_t                 height() const;
    Size                     size() const;
    Point                     pos() const;
    size_t                 fillet() const;
    bool                    alpha() const;
    bool                isVisible() const;
    const Pixmap&            mask() const;
    const Shadow&          shadow() const;
    bool contains(const Point &pos) const;
    Pixmap&                canvas();
    Pixmap                compose();
    virtual void          onPaint();
    void bind(EventType e, const std::function<void()>& fn);
    void activate(EventType e);
    void decorateShadow(int offset_x = 0, int offset_y = 0, double blur = 3.0, double spread = 5.0,
                        xrgb_t color = 0x88000000, bool inset = false);
    // Events processing
    virtual void onMouseEnter  (const MouseEvent        &e);
    virtual void onMouseLeave  (const MouseEvent        &e);
    virtual void onMouseMove   (const MouseMoveEvent    &e);
    virtual void onMousePress  (const MousePressEvent   &e);
    virtual void onMouseRelease(const MouseReleaseEvent &e);
    virtual void onMouseWheel  (const MouseWheelEvent   &e);
    virtual void onKeyPress    (const KeyPressEvent     &e);
    virtual void onKeyRelease  (const KeyReleaseEvent   &e);
    virtual void onClick       (const MouseReleaseEvent &e);
    virtual void onResize      (const WindowResizeEvent &e);
protected:
    std::map<EventType, std::vector<std::function<void()>>> listeners_;
private:
    int         x_, y_, z_;
    Pixmap      content_, mask_;
    size_t      fillet_;
    std::string uuid_;
    WidgetState state_;
    WidgetStyle style_;
    Shadow      shadow_;
};

using SkyWidget = std::shared_ptr<Widget>;

class basic_shadow {
public:
    explicit basic_shadow(Widget& widget, double spread, double blur, xrgb_t color);
    basic_shadow(const basic_shadow&) = default;
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
    Widget &widget_;
    Pixmap shadow_;
    double offsetX_, offsetY_,
           blur_, spread_;
    xrgb_t color_;
    bool   inset_;
};

}


#endif //WIDGET_H
