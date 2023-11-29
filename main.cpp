#include <iostream>
#include "skyui/skyui.h"

void render(jlib::Compositor& compositor);

void image_proc();

class GenericEventListener : public jlib::EventListener {
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
        const auto fontEngine = jlib::FontEngine::GetInstance();
        fontEngine->LoadTTF("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Regular.ttf", fontFamily);
    }
    jlib::Compositor compositor({1280, 960});
    gCompositor = &compositor;
    compositor.framebuffer().fill(0xFFFFAAFF);
    render(compositor);
    jlib::Window window(compositor.framebuffer(), 1000, 1000, "Pretty GUI");
    WindowDisplayBuffer wdb;
    compositor.setDisplay(&wdb);
    gw = &window;
    GenericEventListener listener;
    window.addEventListener(&listener);
    image_proc();
    return app.run(&window);
}

void render(jlib::Compositor& compositor) {
    jlib::View v0({200, 100}), v1({200, 100});
    v0.setFillet(10);
    v0.content().fill(0x88AAFFFF);
    v1.content().fill(0xFFFFAAFF);
    v1.move({50, 50});
    v0.move({100, 100});
    compositor.push(v1);
    compositor.push(v0);
    compositor.flush();
}

void GenericEventListener::listen(const jlib::Event &e) {
    gCompositor->dispatch(e);
}

void WindowDisplayBuffer::flush() {
    gw->update();
}

void image_proc() {
    jlib::View img({100, 100});
    img.content().fill(0x88FFAAAA);
    img.setFillet(10);
    jlib::Painter painter(img.content());
    jlib::Pen pen;
    pen.setColor(0xff333333);
    painter.setPen(&pen);
    painter.drawRect({20, 20}, 20, 20, 0);
    img.content().blur(50, 10);
    img.move({200, 200});
    gCompositor->push(img);
    gCompositor->flush();
}