#include "Application.h"
#include <Magick++/Functions.h>

namespace jlib {

    Application *Application::instance_ = nullptr;

    Application &Application::getInstance() {
        if (!instance_) {
            instance_ = new Application();
        }
        return *instance_;
    }

    void Application::prepare() {
        Magick::InitializeMagick(nullptr);
    }

} // jlib