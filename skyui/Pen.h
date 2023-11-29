#ifndef PEN_H
#define PEN_H

#include <string>
#include "Color.h"

namespace jlib {

class Pen {
public:
    Pen() : color_(0xFFFFFFFF), lineWidth_(1), fontSize_(14), fontFamily_() {}
    Pen(const Pen& pen) = default;
    void setColor     (xrgb_t color) { color_ = color; }
    void setLineWidth (double width) { lineWidth_ = width; }
    void setFontSize  (int size) { fontSize_ = size; }
    void setFontFamily(const std::string& font_family) { fontFamily_ = font_family; }
    xrgb_t     color() const { return color_; }
    double lineWidth() const { return lineWidth_; }
    int     fontSize() const { return fontSize_; }
    const std::string& fontFamily() const { return fontFamily_; }
private:
    xrgb_t color_;
    double lineWidth_;
    int fontSize_;
    std::string fontFamily_;
};

} // jlib

#endif //PEN_H
