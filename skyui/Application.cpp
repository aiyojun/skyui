#include "Application.h"
#include "Window.h"

namespace jlib {

Application::Application(int argc, char* argv[]) {
    Prepare();
}

Application::~Application() {
    Dispose();
}

void Application::Prepare() {

}

void Application::Dispose() {

}


int Application::Run(Window *window) {
    window->Run();
    return 0;
}





} // jlib