#ifndef COLOR_H
#define COLOR_H

#include <vector>

#define PIXEL(ADDR, X, Y, W) ADDR[Y * W + X]
#define DIM_X(xrgb) (xrgb >> 24 & 0xFF)
#define DIM_R(xrgb) (xrgb >> 16 & 0xFF)
#define DIM_G(xrgb) (xrgb >> 8 & 0xFF)
#define DIM_B(xrgb) (xrgb & 0xFF)

namespace jlib {

typedef unsigned int xrgb_t;
typedef unsigned char byte_t;
typedef struct { byte_t r, g, b; double a; } color_t;

color_t mix(color_t _1, color_t _2);
color_t decompose(xrgb_t c);
xrgb_t  compose(color_t c);

class Color {
public:
    static xrgb_t Compose(xrgb_t X, xrgb_t r, xrgb_t g, xrgb_t b)
    { return (r & 0xFF) << 24 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF); }
    static xrgb_t Compose(xrgb_t r, xrgb_t g, xrgb_t b)
    { return (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF); }
    static xrgb_t Mix(const std::vector<xrgb_t>& vec);
    static xrgb_t Blend(xrgb_t x, xrgb_t y, float factor);
    static xrgb_t Strength(xrgb_t xrgb, unsigned char gray);
    static unsigned char GammaCorrect(unsigned char pixel, float gamma);
    static xrgb_t GammaCorrect(xrgb_t pixel, float gamma);
    static xrgb_t Invert(xrgb_t pixel);
    static double Percent(unsigned char gray);
};

} // jlib

#endif //COLOR_H
