#ifndef BUTTON_H
#define BUTTON_H

#include "skyui.h"

namespace jlib {

    class Button : public Widget {
    public:
        explicit Button(std::string text, const Size& size = {100, 32});
        ~Button() override = default;
        void setText(const std::string& text) { text_ = text; }
        const std::string& getText() const { return text_; }
        void onPaint() override;
        void onMouseEnter(const jlib::MouseEvent &e) override;
        void onMouseLeave(const jlib::MouseEvent &e) override;
        void onMousePress(const jlib::MousePressEvent &e) override;
        void onMouseRelease(const jlib::MouseReleaseEvent &e) override;
    private:
        std::string text_;
        bool hover_;
        bool pressed_;
    };

} // jlib

#endif //BUTTON_H
