#ifndef IMAGEENGINE_H
#define IMAGEENGINE_H

#include <memory>
#include "Pixmap.h"

namespace jlib {

class ImageEngine {
public:
    ImageEngine();

    Pixmap Read(const char *filename);

    void Blur(Pixmap& pixmap, int radius, int sigma);
};

}



#endif //IMAGEENGINE_H
