#ifndef PAINTER_H
#define PAINTER_H

#include <cairo/cairo.h>
#include <string>

#include "Pixmap.h"
#include "basic_ds.h"
#include "Font.h"

namespace jlib {

class Pen {
public:
    Pen() : color_(0xFFFFFFFF), lineWidth_(1), fontSize_(14), fontFamily_() {}
    Pen(const Pen& pen) = default;
    void setColor     (xrgb_t color) { color_ = color; }
    void setLineWidth (double width) { lineWidth_ = width; }
    void setFontSize  (int size) { fontSize_ = size; }
    void setFontFamily(const std::string& font_family) { fontFamily_ = font_family; }
    xrgb_t     color() const { return color_; }
    double lineWidth() const { return lineWidth_; }
    int     fontSize() const { return fontSize_; }
    const std::string& fontFamily() const { return fontFamily_; }
private:
    xrgb_t color_;
    double lineWidth_;
    int fontSize_;
    std::string fontFamily_;
};

class Painter {
public:
    explicit Painter(const Pixmap& canvas);
    ~Painter();
    void drawLine(Point b, Point e);
    void drawText(Point b, const std::string& text);
    void drawText(Point b, const std::string& text, const Font& font);
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
