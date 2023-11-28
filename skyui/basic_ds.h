#ifndef BASIC_DS_H
#define BASIC_DS_H

#include <cstddef>

namespace jlib {

    class Size {
    public:
        Size() = default;

        Size(const Size &) = default;

        size_t width, height;
    };

    class Point {
    public:
        Point() = default;

        Point(const Point &) = default;

        int x, y;
    };

} // jlib

#endif //BASIC_DS_H
