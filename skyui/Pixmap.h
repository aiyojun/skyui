#ifndef BITMAP_H
#define BITMAP_H

#include <memory>
#include "Color.h"
#include "basic_ds.h"

namespace jlib {

class basic_pixmap {
public:
    basic_pixmap();
    basic_pixmap(const basic_pixmap& pxm) = default;
    basic_pixmap(size_t w, size_t h);
    explicit basic_pixmap(const Size& sz);
    basic_pixmap& operator=(const basic_pixmap& pxm);
    bool    empty() const;
    size_t  width() const;
    size_t height() const;
    xrgb_t  *data() const;
    xrgb_t   *ref() const;
    Size     size() const;
    void load(const std::string& filename);
    void blur(double radius, double sigma);
    void fill(xrgb_t color);
    void fillRect(xrgb_t color, const Point& b, const Point& e);
    void blend(const basic_pixmap& pxm, const Point& pos);
    void cover(const basic_pixmap& pxm, const Point& pos);
    basic_pixmap copy() const;
    basic_pixmap mask(const basic_pixmap& msk) const
    { return basic_pixmap::mask(*this, msk); }
    static basic_pixmap mask(const basic_pixmap& pxm, const basic_pixmap& msk);
    std::string to_string() const;
private:
    class PrivateStorage;
    using AutoStorage = std::shared_ptr<PrivateStorage>;
    AutoStorage ref_;
};

using Pixmap = basic_pixmap;

} // jlib

#endif //BITMAP_H
