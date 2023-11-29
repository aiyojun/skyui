#include "Button.h"
#include <utility>

namespace jlib {

    Button::Button(std::string text, const Size& size) : Widget(size), text_(std::move(text)) {
        Widget::paint();
        setFillet(6);
        content().fill(0xFF409EFF);
        setShadow();
        Painter painter(content());
        Pen pen;
        pen.setColor(0xFFFFEEEE);
        pen.setFontSize(16);
        pen.setFontFamily("JetBrains Mono");
        painter.setPen(&pen);
        painter.drawText({0, 16}, text_);
    }

    void Button::paint() {

    }

} // jlib