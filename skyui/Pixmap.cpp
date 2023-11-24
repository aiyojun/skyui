#include "Pixmap.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace jlib {

Pixmap::Pixmap(Size size) : width_(size.width), height_(size.height) { Allocate(); }

Pixmap::Pixmap(int w, int h): width_(w), height_(h) { Allocate(); }

Pixmap::~Pixmap() {
}

void Pixmap::Allocate() { data_ = static_cast<xrgb_t *>(malloc(sizeof(xrgb_t) * width_ * height_)); }

void Pixmap::Release() {
    if (data_) {
        free(data_);
        data_ = nullptr;
    }
}

void Pixmap::Fill(xrgb_t color) {
    xrgb_t* ptr = data();
    for (int i = 0; i < width_ * height_; i++)
        ptr[i] = color;
}

void Pixmap::Overlay(Pixmap* dst, const Point& p) {
    Overlay(this, dst, p);
}

void Pixmap::Overlay(Pixmap* src, Pixmap* dst, const Point& p) {
    xrgb_t *srcImg = src->data(), *dstImg = dst->data();
    int w = dst->width_;
    if (p.x + w > src->width_)
        w = src->width_ - p.x;
    for (int j = 0; j < dst->height_; j++) {
        memcpy(srcImg + (p.y + j) * src->width_ + p.x, dstImg + j * dst->width_, sizeof(xrgb_t) * w);
    }
}

XPixmap Pixmap::Shrink(Pixmap* src, int width, int height) {
    XPixmap dst = std::make_shared<Pixmap>(width, height);
    xrgb_t *dstImg = dst->data(), *srcImg = src->data();
    const float xr = (float) src->width_  / width;
    const float yr = (float) src->height_ / height;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            dstImg[i * width + j] =
                srcImg[static_cast<int>(static_cast<double>(i) * yr * src->width_ + static_cast<double>(j) * xr)];
    return dst;
}

XPixmap Pixmap::Joint(Pixmap* bm0, Pixmap* bm1) {
    if (bm0->height_ != bm1->height_)
        throw std::runtime_error("Height must be same");
    XPixmap ret = std::make_shared<Pixmap>(bm0->width_ + bm1->width_, bm0->height());
    const auto bpp = sizeof(xrgb_t);
    for (int j = 0; j < bm0->height_; j++) {
        memcpy(ret->data_ + j * ret->width_, bm0->data_ + j * bm0->width_, bm0->width_ * bpp);
        memcpy(ret->data_ + j * ret->width_ + bm0->width_, bm1->data_ + j * bm1->width_, bm1->width_ * bpp);
    }
    return ret;
}

XPixmap Pixmap::Expand(Pixmap* bm, int top, int bottom, int left, int right) {
    XPixmap dst = std::make_shared<Pixmap>(bm->width_ + left + right, bm->height_ + top + bottom);
//    Pixmap dst(bm.width_ + left + right, bm.height_ + top + bottom);
    const auto bpp = sizeof(xrgb_t);
    for (int j = 0; j < bm->height_; j++) {
        memcmp(dst->data_ + dst->width_ * (j + top) + left, bm->data_ + bm->width_ * j, bm->width_ * bpp);
    }
    return dst;
}

} // jlib