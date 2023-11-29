cmake_minimum_required(VERSION 3.10.0)

project(skyuidemo)

include_directories(
        /usr/include
        /usr/include/cairo
        /usr/include/freetype2
        /usr/include/librsvg-2.0
        /usr/include/ImageMagick-7
        skyui
        uidecl
)

add_executable(skyuidemo
        main.cpp
        skyui/Application.cpp
        skyui/Pixmap.cpp
        skyui/Color.cpp
        skyui/FontEngine.cpp
        skyui/Painter.cpp
        skyui/Pen.cpp
        skyui/Window.cpp
        skyui/View.cpp
        skyui/Compositer.cpp
)
