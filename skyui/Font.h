#ifndef FONT_H
#define FONT_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "basic_ds.h"

namespace jlib {

    class Font {
    public:
        Font() = delete;
        explicit Font(const std::string& filename);
        Font(const Font&) = default;
        ~Font() = default;
        Font& operator=(const Font&) = default;
        const std::string& fontFamily() const;
        size_t fontSize() const { return fontSize_; }
        void setFontSize(size_t fontSize);
    private:
        class PrivateFont;
        using AutoPrivateFont = std::shared_ptr<PrivateFont>;
        size_t fontSize_;
        AutoPrivateFont prv_;
        friend class FontMetric;
        friend class FontEngine;
    };

    class FontEngine {
    public:
        static FontEngine &getInstance();
        ~FontEngine();
        std::string ttf(const std::string& filename);
        Font &getFont(const std::string& fontFamily);
        std::vector<std::string> fonts() const;
    private:
        FontEngine();
        static FontEngine *instance_;
        std::map<std::string, Font> fonts_;
    };

    class FontMetric {
    public:
        static Size measure(const Font& font, const std::string& text);
    };

} // jlib

#endif //FONT_H
