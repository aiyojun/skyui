#include "ImageEngine.h"
#include <Magick++/Functions.h>
#include <Magick++/Image.h>

namespace jlib {

XPixmap ImageEngine::Read(const char *filename) {
    Magick::InitializeMagick(nullptr);
    Magick::Image img;
    img.backgroundColor(Magick::Color(0, 0, 0, 0));
    img.read(filename);
    jlib::XPixmap pixmap = std::make_shared<Pixmap>(img.size().width(), img.size().height());
    img.write(0, 0, img.size().width(), img.size().height(), "BGRA", Magick::StorageType::CharPixel, (void *)pixmap->data());
    return pixmap;
}

}