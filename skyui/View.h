#ifndef VIEW_H
#define VIEW_H


#include "Pixmap.h"
#include "basic_ds.h"

namespace jlib {

class View {
public:
    View(const View&) = default;
    explicit View(const Size& sz);
    void move(const Point& pos) { position_ = pos; }
    void setFillet (size_t radius);
    Pixmap  compose();
    Pixmap& content() { return content_; }
    Point  position() const { return position_; }
    bool      alpha() const { return alpha_; }
    size_t   fillet() const { return fillet_; }
private:
    bool    alpha_;
    Pixmap  content_;
    Point   position_;
    size_t  fillet_;
    int     z_;
};


}


#endif //VIEW_H
