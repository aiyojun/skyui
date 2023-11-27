#include "Pixmap.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace jlib {


//class Pixmap;
//
//using XPixmap = std::shared_ptr<Pixmap>;
//
//class Pixmap {
//public:
//    explicit Pixmap(Size size);
//    Pixmap(int w, int h);
//    ~Pixmap();
//    int width() const { return width_; }
//    int height() const { return height_; }
//    bool isSolid() const { return solid_; }
//    void setSolid(bool solid) { solid_ = solid; }
//    xrgb_t* data() const { return data_; }
//    void Fill(xrgb_t color);
//    void Overlay(Pixmap* dst, const Point& p, bool alphaOpened = false);
//    static void Overlay(Pixmap* src, Pixmap* dst, const Point& p);
//    static void Blend(Pixmap* src, Pixmap* dst, const Point& p);
//    static XPixmap Mask(Pixmap* pixmap, Pixmap *mask);
//    static XPixmap Joint(Pixmap* bm0, Pixmap* bm1);
//    static XPixmap Shrink(Pixmap* src, int width, int height);
//    static XPixmap Expand(Pixmap* bm, int top, int bottom, int left, int right);
//private:
//    void Allocate();
//    void Release();
//    xrgb_t *data_;
//    int width_;
//    int height_;
//    bool solid_;
//};



//Pixmap::Pixmap(Size size) : width_(size.width), height_(size.height) { Allocate(); }
//
//Pixmap::Pixmap(int w, int h): width_(w), height_(h) { Allocate(); }
//
//Pixmap::~Pixmap() {
//}
//
//void Pixmap::Allocate() { data_ = static_cast<xrgb_t *>(malloc(sizeof(xrgb_t) * width_ * height_)); }
//
//void Pixmap::Release() {
//    if (data_) {
//        free(data_);
//        data_ = nullptr;
//    }
//}
//
//void Pixmap::Fill(xrgb_t color) {
//    xrgb_t* ptr = data();
////    ::memset(ptr, (int) color, sizeof(xrgb_t) * width_ * height_);
//    for (int i = 0; i < width_; i++)
//        ptr[i] = color;
//    auto unit = sizeof(xrgb_t);
//    for (int i = 1; i < height_; i++) {
//        ::memcpy(ptr + width_ * i, ptr, unit * width_);
//    }
//}
//
//void Pixmap::Overlay(Pixmap* dst, const Point& p, bool alphaOpened) {
//    if (alphaOpened)
//        Blend(this, dst, p);
//    else
//        Overlay(this, dst, p);
//}
//
//void Pixmap::Overlay(Pixmap* src, Pixmap* dst, const Point& p) {
//    xrgb_t *srcImg = src->data(), *dstImg = dst->data();
//    int w = dst->width_;
//    if (p.x + w > src->width_)
//        w = src->width_ - p.x;
//    for (int j = 0; j < dst->height_; j++) {
//        memcpy(srcImg + (p.y + j) * src->width_ + p.x, dstImg + j * dst->width_, sizeof(xrgb_t) * w);
//    }
//}
//
//void Pixmap::Blend(Pixmap *src, Pixmap *dst, const Point &p) {
//    xrgb_t *srcImg = src->data(), *dstImg = dst->data();
//    for (int j = 0; j < dst->height_; j++) {
//        for (int i = 0; i < dst->width_; i++) {
//            if (p.x + i >= 0 && p.y + j >=0 && p.x + i < src->width_ && p.y + j < src->height_) {
//                auto& bc = srcImg[src->width_ * (p.y + j) + p.x + i];
//                auto& fc = dstImg[dst->width_ * j + i];
//                bc = Color::Blend(bc, fc);
//            }
//        }
//    }
//}
//
//XPixmap Pixmap::Shrink(Pixmap* src, int width, int height) {
//    XPixmap dst = std::make_shared<Pixmap>(width, height);
//    xrgb_t *dstImg = dst->data(), *srcImg = src->data();
//    const float xr = (float) src->width_  / width;
//    const float yr = (float) src->height_ / height;
//    for (int i = 0; i < height; i++)
//        for (int j = 0; j < width; j++)
//            dstImg[i * width + j] =
//                srcImg[static_cast<int>(static_cast<double>(i) * yr * src->width_ + static_cast<double>(j) * xr)];
//    return dst;
//}
//
//XPixmap Pixmap::Joint(Pixmap* bm0, Pixmap* bm1) {
//    if (bm0->height_ != bm1->height_)
//        throw std::runtime_error("Height must be same");
//    XPixmap ret = std::make_shared<Pixmap>(bm0->width_ + bm1->width_, bm0->height());
//    const auto bpp = sizeof(xrgb_t);
//    for (int j = 0; j < bm0->height_; j++) {
//        memcpy(ret->data_ + j * ret->width_, bm0->data_ + j * bm0->width_, bm0->width_ * bpp);
//        memcpy(ret->data_ + j * ret->width_ + bm0->width_, bm1->data_ + j * bm1->width_, bm1->width_ * bpp);
//    }
//    return ret;
//}
//
//XPixmap Pixmap::Expand(Pixmap* bm, int top, int bottom, int left, int right) {
//    XPixmap dst = std::make_shared<Pixmap>(bm->width_ + left + right, bm->height_ + top + bottom);
////    Pixmap dst(bm.width_ + left + right, bm.height_ + top + bottom);
//    const auto bpp = sizeof(xrgb_t);
//    for (int j = 0; j < bm->height_; j++) {
//        memcmp(dst->data_ + dst->width_ * (j + top) + left, bm->data_ + bm->width_ * j, bm->width_ * bpp);
//    }
//    return dst;
//}
//
//XPixmap Pixmap::Mask(Pixmap* pixmap, Pixmap *mask) {
//    if (pixmap->width_ != mask->width_ || pixmap->height_ != mask->height_)
//        throw std::runtime_error("mask error");
//    XPixmap dst = std::make_shared<Pixmap>(pixmap->width_, pixmap->height_);
//    xrgb_t *ptr = dst->data_, *raw = pixmap->data_, *msk = mask->data_;
//    size_t width = mask->width_;
//    for (int j = 0; j < mask->height_; j++) {
//        for (int i = 0; i < width; i++) {
//            auto index = width * j + i;
//            ptr[index] = (0xFF000000 & msk[index]) | (0x00FFFFFF & raw[index]);
//        }
//    }
//    return dst;
//}

//XPixmap Pixmap::CreateRoundRectangle(int width, int height, int r) {
//    XPixmap dst = std::make_shared<Pixmap>(width, height);
//    return dst;
//}

    basic_pixmap::Storage::Storage(Size sz) : width_(sz.width), height_(sz.height) {
        ptr_ = (xrgb_t *) ::malloc(sizeof(xrgb_t) * sz.width * sz.height);
    }

    basic_pixmap::Storage::~Storage() {
        if (ptr_) {
            ::free(ptr_);
            ptr_ = nullptr;
        }
    }

    basic_pixmap &basic_pixmap::operator=(const basic_pixmap &pxm) {
        ref_ = pxm.ref_;
        return *this;
    }

    void basic_pixmap::overlay(const basic_pixmap &pxm, const Point &pos) {
        xrgb_t *srcImg = data(), *dstImg = pxm.data();
        int w = pxm.width();
        if (pos.x + w > width())
            w = width() - pos.x;
        for (int j = 0; j < pxm.height(); j++) {
            memcpy(srcImg + (pos.y + j) * width() + pos.x, dstImg + j * pxm.width(), sizeof(xrgb_t) * w);
        }
    }

    void basic_pixmap::blend(const basic_pixmap &pxm, const Point &p) {
        xrgb_t *srcImg = data(), *dstImg = pxm.data();
        for (int j = 0; j < pxm.height(); j++) {
            for (int i = 0; i < pxm.width(); i++) {
                if (p.x + i >= 0 && p.y + j >=0 && p.x + i < width() && p.y + j < height()) {
                    auto& bc = srcImg[width() * (p.y + j) + p.x + i];
                    auto& fc = dstImg[width() * j + i];
                    bc = Color::Blend(bc, fc);
                }
            }
        }
    }

    basic_pixmap basic_pixmap::mask(const basic_pixmap &pixmap, const basic_pixmap &mask) {
        if (pixmap.width() != mask.width() || pixmap.height() != mask.height())
            throw std::runtime_error("mask error");
        basic_pixmap dst(pixmap.width(), pixmap.height());
        xrgb_t *ptr = dst.data(), *raw = pixmap.data(), *msk = mask.data();
        size_t width = mask.width();
        for (int j = 0; j < mask.height(); j++) {
            for (int i = 0; i < width; i++) {
                auto index = width * j + i;
                ptr[index] = (0xFF000000 & msk[index]) | (0x00FFFFFF & raw[index]);
            }
        }
        return dst;
    }

    void basic_pixmap::fill(xrgb_t color) {
        const auto pxm_width = width(), pxm_height = height();
        xrgb_t* ptr = data();
        for (int i = 0; i < pxm_width; i++)
            ptr[i] = color;
        auto unit = sizeof(xrgb_t);
        for (int i = 1; i < pxm_height; i++) {
            ::memcpy(ptr + pxm_width * i, ptr, unit * pxm_width);
        }
    }

} // jlib