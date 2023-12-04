#include "Button.h"
#include <utility>
#include <algorithm>

namespace jlib {

    Button::Button(std::string text, const Size &size)
            : Widget(size, 6), text_(std::move(text)), hover_(false), pressed_(false) {
        decorateShadow(3, 3);
    }

    void Button::onPaint() {
        canvas().fill(hover_ ? (pressed_ ? 0xFF3A8EE6 : 0xFF66B1FF) : 0xFF409EFF);
        Painter painter(canvas());
        Pen pen;
        pen.setColor(0xFFFFEEEE);
        pen.setFontSize(16);
        pen.setFontFamily("JetBrains Mono");
        painter.setPen(&pen);
        auto font = FontEngine::getInstance().getFont("JetBrains Mono");
        Size textSize = FontMetric::measure(font, text_);
        painter.drawText({
            (int) (((double) width() - (double) textSize.width) * 0.5),
            (int) (((double) height() + (double) textSize.height) * 0.5)
            },text_, FontEngine::getInstance().getFont("JetBrains Mono"));
    }

    void Button::onMouseEnter(const MouseEvent &e) {
        hover_ = true;
    }

    void Button::onMouseLeave(const MouseEvent &e) {
        hover_ = false;
    }

    void Button::onMousePress(const MousePressEvent &e) {
        pressed_ = true;
    }

    void Button::onMouseRelease(const MouseReleaseEvent &e) {
        pressed_ = false;
    }

    void Button::onClick(const MouseReleaseEvent &e) {
        activate(EventType::ETClick);
    }

    void Button::clicked(const std::function<void()>& hd) {
        bind(EventType::ETClick, hd);
    }

} // jlib