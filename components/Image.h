#ifndef IMAGE_H
#define IMAGE_H

#include "Widget.h"

namespace jlib {

    class Image : public Widget {
    public:
        explicit Image(const std::string& filename);
    };

} // jlib

#endif //IMAGE_H
