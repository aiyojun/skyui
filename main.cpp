#include <iostream>
#include "skyui/skyui.h"
#include "components/Button.h"

class GenericEventListener : public jlib::IEventListener {
public:
    void listen(const jlib::Event& e) override;
};

class WindowDisplayBuffer : public jlib::IBuffer {
public:
    void flush() override;
};

jlib::Compositor* gCompositor;
jlib::Window* gw;

int main(int argc, char* argv[]) {
    jlib::Application app(argc, argv);
    std::string fontFamily;
    { /** load font */
        fontFamily = jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Regular.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Bold.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-ExtraBold.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Italic.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Bold-Italic.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-ExtraBold-Italic.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Medium.ttf");
        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Medium-Italic.ttf");
        auto font = jlib::FontEngine::getInstance().getFont(fontFamily);
        std::string text = "ok";
        auto size = jlib::FontMetric::measure(font, text);
        printf("[Font] Measure \"%s\", font size : %d, width : %d px, height : %d px\n", text.c_str(), (int) font.fontSize(), (int) size.width, (int) size.height);
    }
    printf("[Font] %s\n", fontFamily.c_str());
    jlib::Compositor compositor({600, 400});
    gCompositor = &compositor;
    compositor.framebuffer().fill(0xFFFFAAFF);

    jlib::Button btn0("ok");
    btn0.move(100, 50);
    compositor.push(btn0);
    compositor.flush();

    jlib::Window window(compositor.framebuffer(), 1000, 1000, "Pretty GUI");
    WindowDisplayBuffer wdb;
    compositor.setDisplay(&wdb);
    gw = &window;
    GenericEventListener listener;
    window.addEventListener(&listener);
    return app.run(&window);
}


void GenericEventListener::listen(const jlib::Event &e) {
    gCompositor->dispatch(e);
}

void WindowDisplayBuffer::flush() {
    gw->update();
}
