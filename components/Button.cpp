#include "Button.h"
#include <utility>

namespace jlib {

    Button::Button(std::string text, const Size& size) : Widget(size, 6), text_(std::move(text)) {
        content().fill(0xFF409EFF);
        decorateShadow(3, 3);
        Painter painter(content());
        Pen pen;
        pen.setColor(0xFFFFEEEE);
        pen.setFontSize(16);
        pen.setFontFamily("JetBrains Mono");
        painter.setPen(&pen);
        auto font = FontEngine::getInstance().getFont("JetBrains Mono");
        Size textSize = FontMetric::measure(font, text_);
        painter.drawText({
            (int) (((double) size.width - (double) textSize.width) * 0.5),
            (int) (((double) size.height + (double) textSize.height) * 0.5)},
            text_, FontEngine::getInstance().getFont("JetBrains Mono"));
    }

    void Button::paint() {

    }

} // jlib