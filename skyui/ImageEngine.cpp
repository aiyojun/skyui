#include "ImageEngine.h"
#include <Magick++/Functions.h>
#include <Magick++/Image.h>

namespace jlib {

ImageEngine::ImageEngine() {
    Magick::InitializeMagick(nullptr);
}

Pixmap ImageEngine::Read(const char *filename) {
    Magick::Image img;
    img.backgroundColor(Magick::Color(0, 0, 0, 0));
    img.read(filename);
    Pixmap pixmap(img.size().width(), img.size().height());
    img.write(0, 0,
              img.size().width(),
              img.size().height(),
              "BGRA",
              Magick::StorageType::CharPixel,
              (void *)pixmap.data());
    return pixmap;
}

void ImageEngine::Blur(Pixmap &pixmap, int radius, int sigma) {
    Magick::Image img;
    img.backgroundColor(Magick::Color(0, 255, 255, 255));
    img.read(pixmap.width(),
             pixmap.height(),
             "ARGB",
             Magick::CharPixel,
             (void *) pixmap.data());
    img.blur(radius, sigma);
}

}



