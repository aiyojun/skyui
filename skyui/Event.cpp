
#include "Event.h"


namespace jlib {

    Mouse *Mouse::instance_ = nullptr;

    const Mouse &Mouse::getInstance() {
        if (!instance_)
            instance_ = new Mouse;
        return *instance_;
    }

    Keyboard *Keyboard::instance_ = nullptr;

    const Keyboard &Keyboard::getInstance() {
        if (!instance_)
            instance_ = new Keyboard;
        return *instance_;
    }

}

