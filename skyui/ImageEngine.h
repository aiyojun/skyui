#ifndef IMAGEENGINE_H
#define IMAGEENGINE_H

#include <memory>
#include "Pixmap.h"

namespace jlib {

class ImageEngine {
public:
    XPixmap Read(const char *filename);
};

}



#endif //IMAGEENGINE_H
