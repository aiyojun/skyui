#ifndef SKYUIDEMO_COMPOSITOR_H
#define SKYUIDEMO_COMPOSITOR_H


#include <map>
#include "View.h"
#include "Pixmap.h"

namespace jlib {

class Compositor {
public:
    Compositor(int w, int h);
    ~Compositor();
    void flush();
    void addView(View *v) { stack_.emplace_back(v); }
    Pixmap *framebuffer() const { return framebuffer_; }
private:
    void clean();
    std::vector<View *> stack_;
    Pixmap *framebuffer_;
};

};

#endif //SKYUIDEMO_COMPOSITOR_H
