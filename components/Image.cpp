#include "Image.h"

namespace jlib {

    Image::Image(const std::string& filename) : Widget(Size(0, 0)) {
        canvas().load(filename);
    }

} // jlib