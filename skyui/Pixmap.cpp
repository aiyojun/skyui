#include "Pixmap.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <Magick++/Image.h>

namespace jlib {

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

    void basic_pixmap::cover(const basic_pixmap &pxm, const Point &pos) {
        xrgb_t *srcImg = data(), *dstImg = pxm.data();
        size_t srcWidth = width();
        size_t w = pxm.width();
        if (pos.x + w > srcWidth)
            w = srcWidth - pos.x;
        for (int j = 0; j < pxm.height(); j++) {
            memcpy(srcImg + (pos.y + j) * srcWidth + pos.x, dstImg + j * pxm.width(), sizeof(xrgb_t) * w);
        }
    }

    void basic_pixmap::blend(const basic_pixmap &pxm, const Point &p) {
        xrgb_t *srcImg = data(), *dstImg = pxm.data();
        size_t srcWidth = width();
        for (int j = 0; j < pxm.height(); j++) {
            for (int i = 0; i < pxm.width(); i++) {
                if (p.x + i >= 0 && p.y + j >= 0 && p.x + i < srcWidth && p.y + j < height()) {
                    auto& bc = srcImg[srcWidth * (p.y + j) + p.x + i];
                    auto& fc = dstImg[pxm.width() * j + i];
                    bc = ColorUtil::blend(bc, fc); // Color::Blend(bc, fc);
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
                auto xc = DIM_X(msk[index]);
                if (xc == 0) {
                    ptr[index] = 0x00FFFFFF & raw[index];
                }else {
                    ptr[index] = ColorUtil::alpha(raw[index], (float) xc / 255); // Color::Transparent(raw[index], (float) xc / 255);
                }
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

    void basic_pixmap::load(const std::string &filename) {
        Magick::Image img;
        img.backgroundColor(Magick::Color(255, 255, 255, 0));
        img.read(filename);
        size_t w = img.size().width(), h = img.size().height();
        *this = Pixmap(w, h);
        img.write(0, 0, w, h, "BGRA", Magick::CharPixel, (void *) data());
    }

    void basic_pixmap::blur(double radius, double sigma) {
        size_t w = width(), h = height();
        Magick::Image img;
        img.backgroundColor(Magick::Color(255, 255, 255, 0));
        img.read(w, h, "BGRA", Magick::CharPixel, (void *) data());
        img.blur(radius, sigma);
        img.write(0, 0, w, h, "BGRA", Magick::CharPixel, (void *) data());
    }

    void basic_pixmap::fillRect(xrgb_t color, const Point &b, const Point &e) {
        const auto raw_width = width(), raw_height = height();
        if (b.x < 0 || b.y < 0 || e.x < 0 || e.y < 0
            || b.x >= raw_width || b.y >= raw_height || e.x >= raw_width || e.y >= raw_height
            || b.x > e.x || b.y > e.y)
            throw std::runtime_error("rectangle arguments error");
        const auto delta_width = e.x - b.x, delta_height = e.y - b.y;
        xrgb_t* ptr = data();
        for (int i = 0; i < delta_width; i++)
            ptr[b.y * raw_width + b.x + i] = color;
        auto unit = sizeof(xrgb_t);
        for (int i = 1; i < delta_height; i++) {
            ::memcpy(ptr + raw_width * (b.y + i) + b.x, ptr + raw_width * b.y + b.x, unit * delta_width);
        }
    }

    basic_pixmap basic_pixmap::copy() const {
        basic_pixmap bak(size());
        ::memcpy(bak.data(), data(), width() * height() * sizeof(xrgb_t));
        return bak;
    }

} // jlib