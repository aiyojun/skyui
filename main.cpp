#include <iostream>

#include "skyui/skyui.h"

int main(int argc, char* argv[]) {
    jlib::Application app(argc, argv);
    std::string fontFamily;
    { /** load font */
        const auto fontEngine = jlib::FontEngine::GetInstance();
        fontEngine->LoadTTF("/root/Downloads/JetBrainsMono-1.0.3/ttf/JetBrainsMono-Regular.ttf", fontFamily);
    }
    jlib::ImageEngine image_engine;
    std::cout << "-- Load font : " << fontFamily << std::endl;
    jlib::Pixmap surface(800, 600);
    surface.Fill(0xFFEEEEEE);
//    std::shared_ptr<jlib::Pixmap> image = image_engine.ReadAsBitmap("/root/Pictures/demo.png");
    { /** painting */
        jlib::Painter painter(&surface);
        jlib::Pen pen;
        pen.UseLight();
        pen.set_font_family(fontFamily);
        pen.set_font_size(12);
        painter.set_pen(&pen);
        std::shared_ptr<jlib::Pixmap> image = image_engine.Read("/root/jpro/gwf/home.svg");
//        image_engine.Read("/root/Pictures/demo.png");
//        painter.set_primary_color(0x00000000);
        // painter.DrawLine({0, 0}, {100, 50});
        // painter.DrawLineAntialiased({0, 50}, {100, 100});
        // painter.DrawLine2({0, 150}, {100, 200});
        // painter.DrawLine3({0, 100}, {100, 150});
        std::cout << "-- draw image" << std::endl;
//        painter.DrawImage({100, 0}, {320, 180}, image.get());
//        painter.DrawImage({100, 0}, image.get(), .25);
         painter.DrawImage({100, 0}, {24, 24}, image.get());
        std::cout << "-- draw text" << std::endl;
        painter.DrawText({10, 50}, "Hello World");
//        painter.DrawText("Hello World", {10, 50}, fontFamily, 13, false);
//        std::cout << "-- draw rect" << std::endl;
//        painter.DrawRect({50, 50}, {50, 30}, 0x00EEAAEE);
    }
    jlib::Window window(&surface, 0, 0, "Pretty GUI");
    return app.Run(&window);
}
