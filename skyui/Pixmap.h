#ifndef BITMAP_H
#define BITMAP_H

#include <memory>
#include "Point.h"
#include "Size.h"
#include "Color.h"

namespace jlib {

class Pixmap;

using XPixmap = std::shared_ptr<Pixmap>;

class Pixmap {
public:
    explicit Pixmap(Size size);
    Pixmap(int w, int h);
    ~Pixmap();
    int width() const { return width_; }
    int height() const { return height_; }
    bool isSolid() const { return solid_; }
    void setSolid(bool solid) { solid_ = solid; }
    xrgb_t* data() const { return data_; }
    void Fill(xrgb_t color);
    void Overlay(Pixmap* dst, const Point& p);
    static void Overlay(Pixmap* src, Pixmap* dst, const Point& p);
    static XPixmap Joint(Pixmap* bm0, Pixmap* bm1);
    static XPixmap Shrink(Pixmap* src, int width, int height);
    static XPixmap Expand(Pixmap* bm, int top, int bottom, int left, int right);
private:
    void Allocate();
    void Release();
    xrgb_t *data_;
    int width_;
    int height_;
    bool solid_;
};



} // jlib

#endif //BITMAP_H
