#ifndef BASIC_TRAIT_H
#define BASIC_TRAIT_H

namespace jlib {

    class IBuffer {
    public:
        virtual void flush() = 0;
    };

};

#endif //BASIC_TRAIT_H
