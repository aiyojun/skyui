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
    Dispose();
}

void Painter::Prepare() {

}

void Painter::Dispose() {
    cairo_destroy(storage_.cr);
    cairo_surface_destroy(storage_.surface);
}

void Painter::Clear(xrgb_t color) {
    const int size = canvas_.width() * canvas_.height();
    xrgb_t *fb = canvas_.data();
    for (int i = 0; i < size; i++)
        fb[i] = color;
}

void Painter::DrawLine(Point p0, Point p1) {
    cairo_move_to(storage_.cr, p0.x, p0.y);
    cairo_line_to(storage_.cr, p1.x, p1.y);
}

void Painter::DrawText(Point p, const std::string& text) {
    cairo_set_source_rgba(storage_.cr,
                          Color::Percent(DIM_R(pen_->color())),
                          Color::Percent(DIM_G(pen_->color())),
                          Color::Percent(DIM_B(pen_->color())),
                          Color::Percent(DIM_X(pen_->color())));
    cairo_set_font_size(storage_.cr, pen_->font_size());
    cairo_move_to(storage_.cr, p.x, p.y);
    cairo_show_text(storage_.cr, text.c_str());
}

void Painter::DrawCurve(Point b, Point c0, Point c1, Point c2) {
    cairo_set_source_rgba(storage_.cr,
                          Color::Percent(DIM_R(pen_->color())),
                          Color::Percent(DIM_G(pen_->color())),
                          Color::Percent(DIM_B(pen_->color())),
                          Color::Percent(DIM_X(pen_->color())));
    cairo_set_line_width(storage_.cr, pen_->line_width());
    cairo_move_to(storage_.cr, b.x, b.y);
    cairo_curve_to(storage_.cr, c0.x, c0.y, c1.x, c1.y, c2.x, c2.y);
}

void Painter::DrawImage(const Point& pos, const Size& size, const std::string& filename) {
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

void Painter::DrawImage(const Point &pos, const Size &size, Pixmap *pixmap) {
    cairo_surface_t *orig_image = cairo_image_surface_create_for_data((unsigned char *) pixmap->data(), CAIRO_FORMAT_ARGB32, pixmap->width(), pixmap->height(), pixmap->width() * 4);
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

void Painter::DrawImage(const Point &pos, Pixmap *pixmap, double ratio) {
    cairo_surface_t *orig_image = cairo_image_surface_create_for_data((unsigned char *) pixmap->data(), CAIRO_FORMAT_ARGB32, pixmap->width(), pixmap->height(), pixmap->width() * 4);
    int orig_width = cairo_image_surface_get_width(orig_image), orig_height = cairo_image_surface_get_height(orig_image);
    cairo_surface_t *image = cairo_surface_create_similar(orig_image, CAIRO_CONTENT_COLOR_ALPHA, (int) (orig_width * ratio), (int) (orig_height * ratio));
    cairo_t *cr = cairo_create(image);
    cairo_scale(cr, ratio, ratio);
    cairo_set_source_surface(cr, orig_image, 0, 0);
    cairo_paint(cr);
    cairo_set_source_surface(storage_.cr, image, pos.x, pos.y);
    cairo_paint(storage_.cr);
    cairo_destroy(cr);
    cairo_surface_destroy(orig_image);
    cairo_surface_destroy(image);
}

void Painter::DrawRect(Point b, size_t rectWidth, size_t rectHeight, size_t cornerRadius) {
    int x = b.x, y = b.y;
    cairo_move_to(storage_.cr, x + cornerRadius, y);
    cairo_line_to(storage_.cr, x + rectWidth - cornerRadius, y);
    cairo_arc(storage_.cr, x + rectWidth - cornerRadius, y + cornerRadius, cornerRadius, -M_PI_2, 0);
    cairo_line_to(storage_.cr, x + rectWidth, y + rectHeight - cornerRadius);
    cairo_arc(storage_.cr, x + rectWidth - cornerRadius, y + rectHeight - cornerRadius, cornerRadius, 0, M_PI_2);
    cairo_line_to(storage_.cr, x + cornerRadius, y + rectHeight);
    cairo_arc(storage_.cr, x + cornerRadius, y + rectHeight - cornerRadius, cornerRadius, M_PI_2, M_PI);
    cairo_line_to(storage_.cr, x, y + cornerRadius);
    cairo_arc(storage_.cr, x + cornerRadius, y + cornerRadius, cornerRadius, M_PI, -M_PI_2);
    cairo_set_source_rgba(storage_.cr,
                         Color::Percent(DIM_R(pen_->color())),
                         Color::Percent(DIM_G(pen_->color())),
                         Color::Percent(DIM_B(pen_->color())),
                         Color::Percent(DIM_X(pen_->color())));
    cairo_fill(storage_.cr);
}


}
