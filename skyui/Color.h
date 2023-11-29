#ifndef COLOR_H
#define COLOR_H

#include <string>

#define DIM_X(xrgb) (xrgb >> 24 & 0xFF)
#define DIM_R(xrgb) (xrgb >> 16 & 0xFF)
#define DIM_G(xrgb) (xrgb >> 8 & 0xFF)
#define DIM_B(xrgb) (xrgb & 0xFF)

namespace jlib {

typedef unsigned int  xrgb_t;
typedef unsigned char byte_t;

class Color {
public:
    explicit Color(const std::string& rgba);
    Color(byte_t r, byte_t g, byte_t b);
    Color(byte_t r, byte_t g, byte_t b, float alpha);
    Color(const Color&) = default;
    Color& operator=(const Color& c) = default;
    byte_t r() const { return r_; }
    byte_t g() const { return g_; }
    byte_t b() const { return b_; }
    float alpha() const { return alpha_; }
private:
    byte_t r_, g_, b_;
    float alpha_;
};

class ColorUtil {
public:
    static xrgb_t compose(xrgb_t x, xrgb_t r, xrgb_t g, xrgb_t b);
    static xrgb_t fade(xrgb_t rgb, float ratio);
    static xrgb_t alpha(xrgb_t rgb, float alpha);
    static xrgb_t gamma(byte_t pix, float factor); // gamma correct for font
    static xrgb_t blend(xrgb_t bak, xrgb_t frt);
    static xrgb_t invert(xrgb_t rgb);
};

} // jlib

#endif //COLOR_H
