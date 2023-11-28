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
    void Clear(xrgb_t color);
    void DrawLine(Point b, Point e);
    void DrawText(Point b, const std::string& text);
    void DrawCurve(Point b, Point c0, Point c1, Point c2);
    void DrawImage(const Point& pos, const Size& size, const std::string& filename);
    void DrawImage(const Point& pos, const Size& size, Pixmap *image);
    void DrawImage(const Point& pos, Pixmap *image, double ratio);
    void DrawRect(Point b, size_t w, size_t h, size_t r);
    void setPen(Pen *pen) { pen_ = pen; }
private:
    struct CairoStorage { cairo_surface_t *surface; cairo_t *cr; };

    void Prepare();
    void Dispose();

    CairoStorage storage_;
    Pen *pen_;
    Pixmap canvas_;
};

}


#endif //PAINTER_H
