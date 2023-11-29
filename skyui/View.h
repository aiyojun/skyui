#ifndef VIEW_H
#define VIEW_H


#include "Pixmap.h"
#include "basic_ds.h"
#include "Event.h"

namespace jlib {

class View : public EventHandle {
public:
    View(const View&) = default;
    explicit View(const Size& sz);
    void move(int x, int y) { position_ = {x, y}; }
    void move(const Point& pos) { position_ = pos; }
    void setFillet (size_t radius);
    int x() const { return position_.x; }
    int y() const { return position_.y; }
    size_t  width() const { return content_.width(); }
    size_t height() const { return content_.height(); }
    const std::string& uuid() const { return uuid_; }
    Pixmap  compose();
    Pixmap& content() { return content_; }
    Point  position() const { return position_; }
    bool      alpha() const { return alpha_; }
    size_t   fillet() const { return fillet_; }
    bool contains(const Point& pos) const;

    /**
     * @todo:
     *   void show();
     *   void hide();
     *   void onResize(const WindowResizeEvent& e) override;
     *   enum WidgetStyle { SOLID, TRANSLUCENT, ALPHA };
     *   enum WidgetState { NORMAL, HIDDEN };
     *   void setGeometry() {}
     *   void setSize() {}
     */

    void onMouseMove(const MouseMoveEvent& e) override;
    void onMousePress(const MousePressEvent& e) override;
    void onMouseRelease(const MouseReleaseEvent& e) override;
    void onMouseWheel(const MouseWheelEvent& e) override;
    void onKeyPress(const KeyPressEvent& e) override;
    void onKeyRelease(const KeyReleaseEvent & e) override;
    void onClick(const MouseReleaseEvent& e) override;
private:
    std::string uuid_;
    bool    alpha_;
    Pixmap  content_;
    Point   position_;
    size_t  fillet_;
    int     z_;
};


}


#endif //VIEW_H
