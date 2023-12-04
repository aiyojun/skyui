#ifndef APPLICATION_H
#define APPLICATION_H

namespace jlib {

class Application {
public:
    static Application& getInstance();
    Application(const Application&) = delete;
    void prepare();
private:
    Application() = default;
    ~Application() = default;
    static Application *instance_;
};

} // jlib

#endif //APPLICATION_H
