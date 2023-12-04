#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>
#include "Widget.h"

namespace jlib {

	class Window {
	public:
		Window();
		~Window();
        void move(int x, int y);
        void setGeometry(int x, int y, size_t w, size_t h);
        void setTitle(const std::string &title);
        void resize(size_t w, size_t h);
        void show();
        void loop();
        void push(const SkyWidget &widget);
        void remove(const SkyWidget &widget);
        Size size() const;
        Point pos() const;
        std::string title() const;
	private:
		class WindowPrivateImpl;
		std::shared_ptr<WindowPrivateImpl> prv_;
	};

}

#endif // WINDOW_H