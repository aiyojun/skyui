#include "Application.h"
#include "Window.h"

namespace jlib {

Application::Application(int argc, char* argv[]) {
    prepare();
}

Application::~Application() {
    dispose();
}

void Application::prepare() {

}

void Application::dispose() {

}


int Application::run(Window *window) {
    window->run();
    return 0;
}





} // jlib