#ifndef APPLICATION_H
#define APPLICATION_H

namespace jlib {

class Window;
class FontEngine;

class Application {
public:
    Application(int argc, char* argv[]);
    ~Application();

    int run(Window *window);
private:
    void prepare();
    void dispose();
};

} // jlib

#endif //APPLICATION_H
