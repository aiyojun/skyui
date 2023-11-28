#include "Color.h"
#include <cmath>
#include <stdexcept>
#include <regex>

namespace jlib {

color_t mix(color_t _1, color_t _2)
{ return {static_cast<byte_t>((_1.r * _1.a * (1 - _2.a) + _2.r * _2.a) / (_1.a + _2.a - _1.a * _2.a)),
          static_cast<byte_t>((_1.g * _1.a * (1 - _2.a) + _2.g * _2.a) / (_1.a + _2.a - _1.a * _2.a)),
          static_cast<byte_t>((_1.b * _1.a * (1 - _2.a) + _2.b * _2.a) / (_1.a + _2.a - _1.a * _2.a)),
          _1.a + _2.a - _1.a * _2.a}; }

color_t decompose(xrgb_t c)
{ return {(byte_t ) DIM_R(c), (byte_t ) DIM_G(c), (byte_t ) DIM_B(c), (double) DIM_X(c) / 255}; }

//xrgb_t compose(color_t c)
//{ return Color::Compose(c.a, c.r, c.g, c.b); }

//xrgb_t Color::Blend(xrgb_t x, xrgb_t y) {
//    float a0 = (float) DIM_X(x) / 255, a1 = (float) DIM_X(y) / 255;
//    unsigned int r0 = DIM_R(x), r1 = DIM_R(y);
//    unsigned int g0 = DIM_G(x), g1 = DIM_G(y);
//    unsigned int b0 = DIM_B(x), b1 = DIM_B(y);
//    return Compose(255 * (a0 + a1 - a0 * a1),
//                   r0 * a0 * (1 - a1) + r1 * a1,
//                   g0 * a0 * (1 - a1) + g1 * a1,
//                   b0 * a0 * (1 - a1) + b1 * a1);
//}
//
////xrgb_t Color::Blend(xrgb_t x, xrgb_t y, float factor)
////{ return Compose(static_cast<unsigned int>(factor * DIM_R(x) + (1 - factor) * DIM_R(y)),
////                 static_cast<unsigned int>(factor * DIM_G(x) + (1 - factor) * DIM_G(y)),
////                 static_cast<unsigned int>(factor * DIM_B(x) + (1 - factor) * DIM_B(y))); }
//
//xrgb_t Color::Strength(xrgb_t xrgb, unsigned char gray) {
//    const double f = 1.0 * gray / 255;
//    unsigned int r = (xrgb >> 16) & 0xff;
//    unsigned int g = (xrgb >>  8) & 0xff;
//    unsigned int b = xrgb & 0xff;
//    r = static_cast<unsigned int>(f * r) & 0xff;
//    g = static_cast<unsigned int>(f * g) & 0xff;
//    b = static_cast<unsigned int>(f * b) & 0xff;
//    return (r << 16) | (g << 8) | b;
//}
//
//unsigned char Color::GammaCorrect(unsigned char pixel, float gamma)
//{ return static_cast<unsigned char>(std::pow(static_cast<float>(pixel) / 255.0f, 1.0f / gamma) * 255.0f); }
//
//xrgb_t Color::GammaCorrect(xrgb_t pixel, float gamma)
//{ return static_cast<unsigned char>(std::pow(static_cast<float>(pixel) / 255.0f, 1.0f / gamma) * 255.0f); }
//
//xrgb_t Color::Invert(xrgb_t pixel)
//{ return Compose(255 - DIM_R(pixel),  255 - DIM_G(pixel), 255 - DIM_B(pixel)); }
//
//double Color::Percent(unsigned char gray) {return (double) gray / 255; }
//
////xrgb_t Color::Mix(const std::vector<xrgb_t> &vec) {
////    xrgb_t bg = 0xFFFFFFFF;
////    if (vec.empty()) return bg;
////    color_t trans = decompose(bg);
////    for (const auto& each : vec)
////        trans = mix(trans, decompose(each));
////    return compose(trans);
////}
//
//    xrgb_t Color::Transparent(xrgb_t x, float alpha) {
//        return Compose((xrgb_t) (DIM_X(x) * alpha), DIM_R(x), DIM_G(x), DIM_B(x));
//    }

    xrgb_t ColorUtil::compose(xrgb_t x, xrgb_t r, xrgb_t g, xrgb_t b)
    { return (x & 0xFF) << 24 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF); }

    xrgb_t ColorUtil::fade(xrgb_t rgb, float f) {
        unsigned int x = (rgb >> 24) & 0xff;
        unsigned int r = (rgb >> 16) & 0xff;
        unsigned int g = (rgb >>  8) & 0xff;
        unsigned int b = rgb & 0xff;
        r = static_cast<unsigned int>(f * r) & 0xff;
        g = static_cast<unsigned int>(f * g) & 0xff;
        b = static_cast<unsigned int>(f * b) & 0xff;
        return (x << 24) |  (r << 16) | (g << 8) | b;
    }

    xrgb_t ColorUtil::alpha(xrgb_t x, float alpha) {
        return compose((xrgb_t) (DIM_X(x) * alpha), DIM_R(x), DIM_G(x), DIM_B(x));
    }

    xrgb_t ColorUtil::gamma(byte_t pix, float factor)
    { return static_cast<unsigned char>(std::pow(static_cast<float>(pix) / 255.0f, 1.0f / factor) * 255.0f); }

    xrgb_t ColorUtil::blend(xrgb_t bak, xrgb_t frt) {
        float a0 = (float) DIM_X(bak) / 255, a1 = (float) DIM_X(frt) / 255;
        unsigned int r0 = DIM_R(bak), r1 = DIM_R(frt);
        unsigned int g0 = DIM_G(bak), g1 = DIM_G(frt);
        unsigned int b0 = DIM_B(bak), b1 = DIM_B(frt);
        return compose(
                255 * (a0 + a1 - a0 * a1),
                r0 * a0 * (1 - a1) + r1 * a1,
                g0 * a0 * (1 - a1) + g1 * a1,
                b0 * a0 * (1 - a1) + b1 * a1);
    }

    xrgb_t ColorUtil::invert(xrgb_t rgb)
    { return compose(DIM_X(rgb), 255 - DIM_R(rgb),  255 - DIM_G(rgb), 255 - DIM_B(rgb)); }


    Color::Color(const std::string &rgba) {
        if (std::regex_match(rgba, std::regex("#[a-fA-F]{3}"))) {
            r_ = (byte_t) std::stoi(rgba.substr(1, 1), nullptr, 16) << 4;
            g_ = (byte_t) std::stoi(rgba.substr(2, 1), nullptr, 16) << 4;
            b_ = (byte_t) std::stoi(rgba.substr(3, 1), nullptr, 16) << 4;
            alpha_ = 1;
            return;
        }
        if (std::regex_match(rgba, std::regex("#[a-fA-F]{4}"))) {
            r_ = (byte_t) std::stoi(rgba.substr(1, 1), nullptr, 16) << 4;
            g_ = (byte_t) std::stoi(rgba.substr(2, 1), nullptr, 16) << 4;
            b_ = (byte_t) std::stoi(rgba.substr(3, 1), nullptr, 16) << 4;
            alpha_ = (float) (std::stoi(rgba.substr(4, 1), nullptr, 16) << 4) / 255;
            return;
        }
        if (std::regex_match(rgba, std::regex("#[a-fA-F]{6}"))) {
            r_ = (byte_t) std::stoi(rgba.substr(1, 2), nullptr, 16);
            g_ = (byte_t) std::stoi(rgba.substr(3, 2), nullptr, 16);
            b_ = (byte_t) std::stoi(rgba.substr(5, 2), nullptr, 16);
            alpha_ = 1;
            return;
        }
        if (std::regex_match(rgba, std::regex("#[a-fA-F]{8}"))) {
            r_ = (byte_t) std::stoi(rgba.substr(1, 2), nullptr, 16);
            g_ = (byte_t) std::stoi(rgba.substr(3, 2), nullptr, 16);
            b_ = (byte_t) std::stoi(rgba.substr(5, 2), nullptr, 16);
            alpha_ = (float) std::stoi(rgba.substr(5, 2), nullptr, 16) / 255;
            return;
        }
        throw std::runtime_error("unknown color format : " + rgba);
    }

    Color::Color(byte_t r, byte_t g, byte_t b) : r_(r), g_(g), b_(b), alpha_(1) {

    }

    Color::Color(byte_t r, byte_t g, byte_t b, float a) : r_(r), g_(g), b_(b), alpha_(a) {

    }

    Color &Color::operator=(const Color &c) {
        this->r_ = c.r_;
        this->g_ = c.g_;
        this->b_ = c.b_;
        this->alpha_ =c.alpha_;
        return *this;
    }

} // jlib