#include "Font.h"
#include <stdexcept>
#include <vector>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "Color.h"
#include <cairo/cairo.h>

namespace jlib {

    FT_Library ft_lib;

    class Font::PrivateFont {
    public:
        explicit PrivateFont(const std::string& filename);
        ~PrivateFont();
    private:
        std::string fontFamily_;
        FT_Face     face_;
        friend class Font;
        friend class FontMetric;
        friend class FontEngine;
    };

    Font::PrivateFont::PrivateFont(const std::string& filename) {
        face_ = nullptr;
        const FT_Error err = FT_New_Face(ft_lib, filename.c_str(), 0, &face_);
        fontFamily_ = face_->family_name;
        if (err == FT_Err_Unknown_File_Format)
            throw std::runtime_error("Unsupported font format : " + std::string(filename));
        if (err) {
            throw std::runtime_error("Fail to load font : " + std::string(filename));
        }
    }

    Font::PrivateFont::~PrivateFont() {
        FT_Done_Face(face_);
    }

    Font::Font(const std::string &filename)
            : prv_(std::make_shared<PrivateFont>(filename)), fontSize_(16) {

    }

    void Font::setFontSize(size_t fontSize) {
        fontSize_ = fontSize;
    }

    const std::string &Font::fontFamily() const {
        return prv_->fontFamily_;
    }

    FontEngine::FontEngine() {
        const FT_Error err = FT_Init_FreeType(&ft_lib);
        if (err) throw std::runtime_error("Fail to load FreeType library");
    }

    FontEngine::~FontEngine() {
        FT_Done_FreeType(ft_lib);
    }

    FontEngine* FontEngine::instance_ = nullptr;

    FontEngine& FontEngine::getInstance() {
        if (instance_ == nullptr)
            instance_ = new FontEngine();
        return *instance_;
    }

    Font &FontEngine::getFont(const std::string &fontFamily) {
        if (fonts_.find(fontFamily) == fonts_.end()) {
            for (const auto& kv : fonts_) {
                printf("-- font : %s\n", kv.first.c_str());
            }
            throw std::runtime_error(
                    "no such font : " + fontFamily + ", font total : " + std::to_string(fonts_.size()));
        }
        return fonts_.at(fontFamily);
    }

    std::string FontEngine::ttf(const std::string &filename) {
        Font font(filename);
        fonts_.insert_or_assign(font.fontFamily(), font);
        return font.fontFamily();
    }

    std::vector<std::string> FontEngine::fonts() const {
        std::vector<std::string> fonts;
        for (const auto& kv : fonts_)
            fonts.emplace_back(kv.first);
        return fonts;
    }

    Size FontMetric::measure(const Font &font, const std::string &text) {
        cairo_surface_t *surface;
        cairo_t *cr;
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
        cr = cairo_create(surface);
        cairo_select_font_face(cr, font.fontFamily().c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, (double) font.fontSize());
        cairo_text_extents_t extents;
        cairo_text_extents(cr, text.c_str(), &extents);
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
        return {(size_t) extents.width, (size_t) extents.height};
    }

} // jlib