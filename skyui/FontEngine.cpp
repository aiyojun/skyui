#include "FontEngine.h"
#include <stdexcept>
#include <vector>
#include "Color.h"
#include "Point.h"

namespace jlib {

FontEngine::FontEngine() {
    Prepare();
}

FontEngine* FontEngine::GetInstance() {
    if (self_ == nullptr)
        self_ = new FontEngine;
    return self_;
}

FontEngine* FontEngine::self_ = nullptr;

FontEngine::~FontEngine() {
    Dispose();
}

void FontEngine::LoadTTF(const char* filename, std::string& fontname) {
    FT_Face face;
    const FT_Error err = FT_New_Face(lib_, filename, 0, &face);
    fontname = face->family_name;
    face_pool_[fontname] = face;
    if (err == FT_Err_Unknown_File_Format)
        throw std::runtime_error("Unsupported font format : " + std::string(filename));
    if (err) {
        throw std::runtime_error("Fail to load font : " + std::string(filename));
    }
}

void FontEngine::SetFontSize(const std::string& font, int size) {
    const FT_Face& face = face_pool_[font];
    const FT_Error err = FT_Set_Pixel_Sizes(face, 0, size);
    if (err) throw std::runtime_error("Fail to set font size");
}

XPixmap FontEngine::RenderText(const std::string& text, const std::string& font) {
    // XPixmap textImage = std::make_shared<Pixmap>()


    // const FT_Face& face = face_pool_[font];
    // const FT_GlyphSlot& slot = face->glyph;
    //
    // // int width;
    // // int pen_x = p.x, pen_y = p.y;
    // // std::vector<std::shared_ptr<Pixmap>> vec;
    // XPixmap org;
    // FT_Error err;
    // for (const auto&ch: text) {
    //     err = FT_Load_Char(face, ch, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL | FT_LOAD_NO_HINTING);
    //     if (err) continue;
    //     const auto ptr = RenderCharacter(&slot->bitmap);
    //     if (org.get() == nullptr) {
    //         org = ptr;
    //     } else {
    //         const auto nbm = Pixmap::Joint(*org, *ptr);
    //     }
    //     const auto nbm = Pixmap::Joint(*)
    //     // ptr = Pixmap::Joint(*ptr, *RenderCharacter(&slot->bitmap));
    //     // RenderCharacter(canvas, &slot->bitmap, pen_x + slot->bitmap_left, pen_y - slot->bitmap_top, color);
    //     // pen_x += slot->advance.x >> 6;
    //     // pen_y += slot->advance.y >> 6;
    // }


    return {};
}

XPixmap FontEngine::RenderCharacter(FT_Bitmap* bitmap) {
    XPixmap imgChar = std::make_shared<Pixmap>(bitmap->width, bitmap->rows);
    const auto fb = imgChar->data();
    int i, j, p, q;
    for (i = 0, p = 0; i < imgChar->width(); i++, p++) {
        for (j = 0, q = 0; j < imgChar->height(); j++, q++) {
            auto& gray = bitmap->buffer[q * bitmap->width + p];
            gray = Color::GammaCorrect(gray, 1.8); // 2.2
            if (gray > 0) fb[j * imgChar->width() + i] = Color::Strength(0x00FFFFFF, gray);
        }
    }
    return imgChar;
}


void FontEngine::RenderText(Pixmap& canvas, const std::string& text, Point p, xrgb_t color, const std::string& font, bool dark) {
    const FT_Face& face = face_pool_[font];
    const FT_GlyphSlot& slot = face->glyph;
    int pen_x = p.x, pen_y = p.y; FT_Error err;
    for (const auto&ch: text) {
        err = FT_Load_Char(face, ch, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL | FT_LOAD_NO_HINTING);
        if (err) continue;
        RenderCharacter(canvas, &slot->bitmap, pen_x + slot->bitmap_left, pen_y - slot->bitmap_top, color, dark);
        pen_x += slot->advance.x >> 6;
        pen_y += slot->advance.y >> 6;
    }
}

void FontEngine::RenderCharacter(Pixmap& canvas, FT_Bitmap* bitmap, int x, int y, xrgb_t xrgb, bool dark) {
    int i, j, p, q;
    int x_max = x + bitmap->width;
    int y_max = y + bitmap->rows;
    int w = canvas.width(), h = canvas.height();
    xrgb_t *fb = canvas.data();
    for (i = x, p = 0; i < x_max; i++, p++) {
        for (j = y, q = 0; j < y_max; j++, q++) {
            if (i < 0 || j < 0 || i >= w || j >= h) continue;
            auto& gray = bitmap->buffer[q * bitmap->width + p];
            gray = Color::GammaCorrect(gray, 1.8); // 2.2
            if (gray > 0) {
                fb[j * w + i] = dark ? Color::Strength(xrgb, gray) : Color::Invert(Color::Strength(Color::Invert(xrgb), gray));
            }
        }
    }
}

void FontEngine::Prepare() {
    const FT_Error err = FT_Init_FreeType(&lib_);
    if (err) throw std::runtime_error("Fail to load FreeType library");
}

void FontEngine::Dispose() {
    for (const auto&each: face_pool_) {
        FT_Done_Face(each.second);
    }
    FT_Done_FreeType(lib_);
}

} // jlib