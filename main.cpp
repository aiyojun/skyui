#include <iostream>
#include "skyui/skyui.h"
#include "components/Button.h"
#include "components/Image.h"

int main(int argc, char* argv[]) {
    jlib::Application::getInstance().prepare();
    std::string fontFamily;
    { /** load font */
//        fontFamily = jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Regular.ttf");
        fontFamily = jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Bold.ttf");
//        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-ExtraBold.ttf");
//        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Italic.ttf");
//        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Bold-Italic.ttf");
//        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-ExtraBold-Italic.ttf");
//        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Medium.ttf");
//        jlib::FontEngine::getInstance().ttf("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Medium-Italic.ttf");
        auto font = jlib::FontEngine::getInstance().getFont(fontFamily);
        std::string text = "ok";
        auto size = jlib::FontMetric::measure(font, text);
        printf("[Font] Measure \"%s\", font size : %d, width : %d px, height : %d px\n", text.c_str(), (int) font.fontSize(), (int) size.width, (int) size.height);
    }
    printf("[Font] %s\n", fontFamily.c_str());

    jlib::Window window;
    window.setGeometry(800, 600, 400, 200);
    window.setTitle("Sky UI");
    window.show();
    auto bt1 = std::make_shared<jlib::Button>("ok", jlib::Size(120, 32));
    bt1->clicked([]() { printf("ok ...\n"); });
    window.push(bt1);
    bt1->move(50, 50);
//    auto bt2 = std::make_shared<jlib::Button>("cancel", jlib::Size(120, 32));
//    bt2->clicked([]() { printf("cancel ...\n"); });
//    window.push(bt2);
//    bt2->move(200, 50);
    auto img1 = std::make_shared<jlib::Image>("/root/Pictures/demo.png");
    img1->move(0, 80);
    window.push(img1);
    window.loop();
}
