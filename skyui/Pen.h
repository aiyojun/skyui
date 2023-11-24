#ifndef GWF_PEN_H
#define GWF_PEN_H

#include <string>
#include "Color.h"

namespace jlib {

class Pen {
public:
    Pen() : font_family_(""), color_(0x00FFFFFF), font_size_(14), line_width_(1) {}
    Pen(const Pen& pen) : font_family_(pen.font_family_), color_(pen.color_), font_size_(pen.font_size_), line_width_(pen.line_width_) {}
    void set_font_family(const std::string& font_family) { font_family_ = font_family; }
    const std::string& font_family() { return font_family_; }
    void set_color(xrgb_t color) { color_ = color; }
    xrgb_t color() const { return color_; }
    void set_font_size(int size) { font_size_ = size; }
    int font_size() const { return font_size_; }
    void set_line_width(double width) { line_width_ = width; }
    double line_width() const { return line_width_; }
    void UseDark();
    void UseLight();
private:
    xrgb_t color_;
    int font_size_;
    std::string font_family_;
    double line_width_;
};

} // jlib

#endif //GWF_PEN_H
