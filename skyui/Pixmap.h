#ifndef BITMAP_H
#define BITMAP_H

#include <memory>
#include "Color.h"
#include "basic_ds.h"

namespace jlib {

class basic_pixmap {
public:
    basic_pixmap() : ref_() {}
    basic_pixmap(const basic_pixmap& pxm) = default;
    basic_pixmap(size_t w, size_t h) : ref_(std::make_shared<Storage>(Size{w, h})) {}
    explicit basic_pixmap(const Size& sz) : ref_(std::make_shared<Storage>(sz)) {}
    basic_pixmap& operator=(const basic_pixmap& pxm);
    bool    empty() const { return ref_ == nullptr; }
    size_t  width() const { return empty() ? 0 : ref_->width(); }
    size_t height() const { return empty() ? 0 : ref_->height(); }
    xrgb_t  *data() const { return ref_->data(); }
    Size     size() const { return {width(), height()}; }
    xrgb_t   *ref() const { return ref_->data(); }
    void  fill(xrgb_t color);
    void  fillRect(xrgb_t color, const Point& b, const Point& e);
    void  load(const std::string& filename);
    void  blur(double radius, double sigma);
    void blend(const basic_pixmap& pxm, const Point& pos);
    void cover(const basic_pixmap& pxm, const Point& pos);
    basic_pixmap copy() const;
//    basic_pixmap shadow(double blur = 5.0, double spread = 1.5, xrgb_t color = 0xFF000000, bool inset = false);
    basic_pixmap mask(const basic_pixmap& msk) const
    { return basic_pixmap::mask(*this, msk); }
    static basic_pixmap mask(const basic_pixmap& pxm, const basic_pixmap& msk);
private:
    class Storage {
    public:
        explicit Storage(Size sz);
        ~Storage();
        xrgb_t  *data() const { return ptr_; }
        size_t  width() const { return width_; }
        size_t height() const { return height_; }
    private:
        xrgb_t *ptr_;
        size_t width_, height_;
    };
    using AutoStorage = std::shared_ptr<Storage>;
    AutoStorage ref_;
};

using Pixmap = basic_pixmap;

} // jlib

#endif //BITMAP_H
