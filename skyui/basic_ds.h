#ifndef BASIC_DS_H
#define BASIC_DS_H

#include <cstddef>

namespace jlib {

    class Size {
    public:
        Size() = default;

        Size(const Size &) = default;

        Size(size_t w, size_t h) : width(w), height(h) {}

        Size& operator=(const Size&) = default;

        size_t width, height;
    };

    class Point {
    public:
        Point() = default;

        Point(const Point &) = default;

        Point& operator=(const Point&) = default;

        int x, y;
    };

} // jlib

#endif //BASIC_DS_H
