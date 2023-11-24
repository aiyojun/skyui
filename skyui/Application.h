#ifndef APPLICATION_H
#define APPLICATION_H
namespace jlib {

class FontEngine;

class Window;

class Application {
public:
    Application(int argc, char* argv[]);
    ~Application();

    int Run(Window *window);
private:
    void Prepare();
    void Dispose();
};

} // jlib

#endif //APPLICATION_H
