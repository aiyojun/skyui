#include "Color.h"
#include <cmath>

namespace jlib {

color_t mix(color_t _1, color_t _2)
{ return {static_cast<byte_t>((_1.r * _1.a * (1 - _2.a) + _2.r * _2.a) / (_1.a + _2.a - _1.a * _2.a)),
          static_cast<byte_t>((_1.g * _1.a * (1 - _2.a) + _2.g * _2.a) / (_1.a + _2.a - _1.a * _2.a)),
          static_cast<byte_t>((_1.b * _1.a * (1 - _2.a) + _2.b * _2.a) / (_1.a + _2.a - _1.a * _2.a)),
          _1.a + _2.a - _1.a * _2.a}; }

color_t decompose(xrgb_t c)
{ return {(byte_t ) DIM_R(c), (byte_t ) DIM_G(c), (byte_t ) DIM_B(c), (double) DIM_X(c) / 255}; }

xrgb_t compose(color_t c)
{ return Color::Compose(c.a, c.r, c.g, c.b); }

xrgb_t Color::Blend(xrgb_t x, xrgb_t y) {
    float a0 = (float) DIM_X(x) / 255, a1 = (float) DIM_X(y) / 255;
    unsigned int r0 = DIM_R(x), r1 = DIM_R(y);
    unsigned int g0 = DIM_G(x), g1 = DIM_G(y);
    unsigned int b0 = DIM_B(x), b1 = DIM_B(y);
    return Compose(255 * (a0 + a1 - a0 * a1),
                   r0 * a0 * (1 - a1) + r1 * a1,
                   g0 * a0 * (1 - a1) + g1 * a1,
                   b0 * a0 * (1 - a1) + b1 * a1);
}

//xrgb_t Color::Blend(xrgb_t x, xrgb_t y, float factor)
//{ return Compose(static_cast<unsigned int>(factor * DIM_R(x) + (1 - factor) * DIM_R(y)),
//                 static_cast<unsigned int>(factor * DIM_G(x) + (1 - factor) * DIM_G(y)),
//                 static_cast<unsigned int>(factor * DIM_B(x) + (1 - factor) * DIM_B(y))); }

xrgb_t Color::Strength(xrgb_t xrgb, unsigned char gray) {
    const double f = 1.0 * gray / 255;
    unsigned int r = (xrgb >> 16) & 0xff;
    unsigned int g = (xrgb >>  8) & 0xff;
    unsigned int b = xrgb & 0xff;
    r = static_cast<unsigned int>(f * r) & 0xff;
    g = static_cast<unsigned int>(f * g) & 0xff;
    b = static_cast<unsigned int>(f * b) & 0xff;
    return (r << 16) | (g << 8) | b;
}

unsigned char Color::GammaCorrect(unsigned char pixel, float gamma)
{ return static_cast<unsigned char>(std::pow(static_cast<float>(pixel) / 255.0f, 1.0f / gamma) * 255.0f); }

xrgb_t Color::GammaCorrect(xrgb_t pixel, float gamma)
{ return static_cast<unsigned char>(std::pow(static_cast<float>(pixel) / 255.0f, 1.0f / gamma) * 255.0f); }

xrgb_t Color::Invert(xrgb_t pixel)
{ return Compose(255 - DIM_R(pixel),  255 - DIM_G(pixel), 255 - DIM_B(pixel)); }

double Color::Percent(unsigned char gray) {return (double) gray / 255; }

xrgb_t Color::Mix(const std::vector<xrgb_t> &vec) {
    xrgb_t bg = 0xFFFFFFFF;
    if (vec.empty()) return bg;
    color_t trans = decompose(bg);
    for (const auto& each : vec)
        trans = mix(trans, decompose(each));
    return compose(trans);
}

} // jlib