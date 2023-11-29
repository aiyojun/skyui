#include "Pixmap.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <Magick++/Image.h>

namespace jlib {


    void blur(Pixmap pxm, int kernel, double sigma) {

    }

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

    basic_pixmap basic_pixmap::shadow(double blur, double spread, xrgb_t color, bool inset) {
//        spread = 1.5;
        if (spread < 1.0) throw std::runtime_error("error parameter spread : " + std::to_string(spread));
        printf("shadow parameters => blur : %f, spread : %f\n", blur, spread);
        size_t inbox_width   =  width() * 0.5,
               inbox_height  = height() * 0.5,
               outbox_width  = (size_t) (spread * (double) width()),
               outbox_height = (size_t) (spread * (double) height());
//        basic_pixmap inbox ( inbox_width,  inbox_height);
        basic_pixmap outbox(outbox_width, outbox_height);
        outbox.fill(0x00FFFFFF);
        Point p0{(int) ((spread - 1) * 0.5 * (double) width()), (int) ((spread - 1) * 0.5 * (double) height())};
        outbox.fillRect(0xFF000000, p0, {(int) (p0.x + width()), (int) (p0.y + height())});
//        outbox.fillRect(0xFF000000, {10, 10}, {(int) (outbox_width - 10), (int) (outbox_height - 10)});
//        int inbox_x = (int) (0.5 * (double) (outbox_width  - inbox_width)),
//            inbox_y = (int) (0.5 * (double) (outbox_height - inbox_height));
//        inbox.fill(color);
//        inbox.fill(0xFF000000);
//        outbox.cover(inbox, {inbox_x, inbox_y});
//        outbox.blur((double) outbox_width, blur);
//        outbox.blur((double) outbox_width * 2, 10);
//        outbox.cover(*this, {inbox_x, inbox_y});

        Magick::Image img;
        img.backgroundColor(Magick::Color(255, 255, 255, 0));
        img.read(outbox_width, outbox_height, "BGRA", Magick::CharPixel, (void *) outbox.data());
        img.blur(5, 10);
        img.write(0, 0, outbox_width, outbox_height, "BGRA", Magick::CharPixel, (void *) outbox.data());
        return outbox;
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

} // jlib