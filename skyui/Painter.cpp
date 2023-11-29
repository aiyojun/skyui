#include "Painter.h"
#include <cairo-svg.h>
#include <cmath>
#include "Color.h"

namespace jlib {

Painter::Painter(const Pixmap& canvas) : canvas_(canvas), pen_(nullptr), storage_{
    cairo_image_surface_create_for_data(
            (unsigned char *) canvas.data(),
            CAIRO_FORMAT_RGB24, (int) canvas.width(),
            (int) canvas.height(), (int)canvas.width() * 4),
    cairo_create(storage_.surface)
} {

}

Painter::~Painter() {
    dispose();
}

void Painter::dispose() {
    cairo_destroy(storage_.cr);
    cairo_surface_destroy(storage_.surface);
}

void Painter::drawLine(Point b, Point e) {
    cairo_move_to(storage_.cr, b.x, b.y);
    cairo_line_to(storage_.cr, e.x, e.y);
}

void Painter::drawText(Point b, const std::string& text) {
    cairo_set_source_rgba(storage_.cr,
                          (float) (DIM_R(pen_->color())) / 255,
                          (float) (DIM_G(pen_->color())) / 255,
                          (float) (DIM_B(pen_->color())) / 255,
                          (float) (DIM_X(pen_->color())) / 255);
    cairo_set_font_size(storage_.cr, pen_->fontSize());
    cairo_move_to(storage_.cr, b.x, b.y);
    cairo_show_text(storage_.cr, text.c_str());
}

void Painter::drawCurve(Point b, Point c0, Point c1, Point c2) {
    cairo_set_source_rgba(storage_.cr,
                          (float) (DIM_R(pen_->color())) / 255,
                          (float) (DIM_G(pen_->color())) / 255,
                          (float) (DIM_B(pen_->color())) / 255,
                          (float) (DIM_X(pen_->color())) / 255);
    cairo_set_line_width(storage_.cr, pen_->lineWidth());
    cairo_move_to(storage_.cr, b.x, b.y);
    cairo_curve_to(storage_.cr, c0.x, c0.y, c1.x, c1.y, c2.x, c2.y);
}

void Painter::drawImage(const Point& pos, const Size& size, const std::string& filename) {
    cairo_surface_t *orig_image = cairo_image_surface_create_from_png(filename.c_str());
    int orig_width = cairo_image_surface_get_width(orig_image), orig_height = cairo_image_surface_get_height(orig_image);
    double ratio_x = (double) size.width / orig_width, ratio_y  = (double) size.height/ orig_height;
    cairo_surface_t *image = cairo_surface_create_similar(orig_image, CAIRO_CONTENT_COLOR_ALPHA, size.width, size.height);
    cairo_t *cr = cairo_create(image);
    cairo_scale(cr, ratio_x, ratio_y);
    cairo_set_source_surface(cr, orig_image, 0, 0);
    cairo_paint(cr);
    cairo_set_source_surface(storage_.cr, image, pos.x, pos.y);
    cairo_paint(storage_.cr);
    cairo_destroy(cr);
    cairo_surface_destroy(orig_image);
    cairo_surface_destroy(image);
}

void Painter::drawImage(const Point &pos, const Size &size, const Pixmap& image) {
    cairo_surface_t *orig_image = cairo_image_surface_create_for_data((unsigned char *) image.data(), CAIRO_FORMAT_ARGB32, image.width(), image.height(), image.width() * 4);
    int orig_width = cairo_image_surface_get_width(orig_image), orig_height = cairo_image_surface_get_height(orig_image);
    double ratio_x = (double) size.width / orig_width, ratio_y  = (double) size.height/ orig_height;
    cairo_surface_t *img = cairo_surface_create_similar(orig_image, CAIRO_CONTENT_COLOR_ALPHA, size.width, size.height);
    cairo_t *cr = cairo_create(img);
    cairo_scale(cr, ratio_x, ratio_y);
    cairo_set_source_surface(cr, orig_image, 0, 0);
    cairo_paint(cr);
    cairo_set_source_surface(storage_.cr, img, pos.x, pos.y);
    cairo_paint(storage_.cr);
    cairo_destroy(cr);
    cairo_surface_destroy(orig_image);
    cairo_surface_destroy(img);
}

//void Painter::drawImage(const Point &pos, Pixmap *image, double ratio) {
//    cairo_surface_t *orig_image = cairo_image_surface_create_for_data((unsigned char *) image->data(), CAIRO_FORMAT_ARGB32, image->width(), image->height(), image->width() * 4);
//    int orig_width = cairo_image_surface_get_width(orig_image), orig_height = cairo_image_surface_get_height(orig_image);
//    cairo_surface_t *image = cairo_surface_create_similar(orig_image, CAIRO_CONTENT_COLOR_ALPHA, (int) (orig_width * ratio), (int) (orig_height * ratio));
//    cairo_t *cr = cairo_create(image);
//    cairo_scale(cr, ratio, ratio);
//    cairo_set_source_surface(cr, orig_image, 0, 0);
//    cairo_paint(cr);
//    cairo_set_source_surface(storage_.cr, image, pos.x, pos.y);
//    cairo_paint(storage_.cr);
//    cairo_destroy(cr);
//    cairo_surface_destroy(orig_image);
//    cairo_surface_destroy(image);
//}

void Painter::drawRect(Point b, size_t w, size_t h, size_t r) {
    int x = b.x, y = b.y;
    cairo_move_to(storage_.cr, x + r, y);
    cairo_line_to(storage_.cr, x + w - r, y);
    cairo_arc(storage_.cr, x + w - r, y + r, r, -M_PI_2, 0);
    cairo_line_to(storage_.cr, x + w, y + h - r);
    cairo_arc(storage_.cr, x + w - r, y + h - r, r, 0, M_PI_2);
    cairo_line_to(storage_.cr, x + r, y + h);
    cairo_arc(storage_.cr, x + r, y + h - r, r, M_PI_2, M_PI);
    cairo_line_to(storage_.cr, x, y + r);
    cairo_arc(storage_.cr, x + r, y + r, r, M_PI, -M_PI_2);
    cairo_set_source_rgba(storage_.cr,
                         (float)(DIM_R(pen_->color())) / 255,
                         (float)(DIM_G(pen_->color())) / 255,
                         (float)(DIM_B(pen_->color())) / 255,
                         (float)(DIM_X(pen_->color())) / 255);
    cairo_fill(storage_.cr);
}


}
