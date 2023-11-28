#ifndef FONTENGINE_H
#define FONTENGINE_H

#include <map>
#include <memory>
#include <string>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "Pixmap.h"
#include "basic_ds.h"

namespace jlib {

class FontEngine {
public:
    static FontEngine *GetInstance();
    ~FontEngine();
    void LoadTTF(const char *filename, std::string& fontname);
    void SetFontSize(const std::string& font, int size);
    Pixmap RenderText(const std::string& text, const std::string& font);
    void RenderText(Pixmap& canvas, const std::string& text, Point p, xrgb_t color, const std::string& font, bool dark=true);
private:
    FontEngine();
    void Prepare();
    void Dispose();

    Pixmap RenderCharacter(FT_Bitmap* bitmap);
    void RenderCharacter(Pixmap& canvas, FT_Bitmap* bitmap, int x, int y, xrgb_t xrgb, bool dark);

    static FontEngine *self_;

    FT_Library lib_;
    std::map<std::string, FT_Face> face_pool_;
};

} // jlib

#endif //FONTENGINE_H
