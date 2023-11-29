#ifndef PAINTER_H
#define PAINTER_H

#include <cairo/cairo.h>
#include <string>

#include "Pixmap.h"
#include "Pen.h"
#include "basic_ds.h"

namespace jlib {

class Painter {
public:
    explicit Painter(const Pixmap& canvas);
    ~Painter();
    void drawLine(Point b, Point e);
    void drawText(Point b, const std::string& text);
    void drawCurve(Point b, Point c0, Point c1, Point c2);
    void drawImage(const Point& pos, const Size& size, const std::string& filename);
    void drawImage(const Point& pos, const Size& size, const Pixmap& image);
    void drawRect(Point b, size_t w, size_t h, size_t r);
    void setPen(Pen *pen) { pen_ = pen; }
private:
    struct CairoStorage { cairo_surface_t *surface; cairo_t *cr; };
    void   dispose();
    CairoStorage storage_;
    Pixmap canvas_;
    Pen   *pen_;
};

}


#endif //PAINTER_H
