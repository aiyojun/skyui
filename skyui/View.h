#ifndef SKYUIDEMO_VIEW_H
#define SKYUIDEMO_VIEW_H


#include "Pixmap.h"

namespace jlib {

class View {
public:
    View(int w, int h);
    void set_position(int x, int y);
    void set_background_color(xrgb_t c);
    void set_alpha(bool alpha) { alpha_ = alpha; }
    void set_fillet(int f);
    Pixmap *content() { return &content_; }
    Pixmap *mask() { return &mask_; }
    Point position() { return {x_, y_}; }
    xrgb_t background_color() const { return background_color_; }
    bool alpha() const { return alpha_; }
    int fillet() const { return fillet_; }
private:
    void Prepare();
    int x_, y_, z_;  // , width_, height_;
    int fillet_;
    bool alpha_;
    xrgb_t background_color_;
    Pixmap content_;
    Pixmap mask_;
};

}


#endif //SKYUIDEMO_VIEW_H
