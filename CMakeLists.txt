cmake_minimum_required(VERSION 3.10.0)

project(skyuidemo)

include_directories(
        /usr/include
        /usr/include/cairo
        /usr/include/freetype2
        /usr/include/librsvg-2.0
        /usr/include/ImageMagick-7
        skyui
)

add_executable(skyuidemo
        main.cpp
        skyui/Application.cpp
        skyui/Pixmap.cpp
        skyui/Color.cpp
        skyui/Font.cpp
        skyui/Painter.cpp
#        skyui/Window.cpp
        skyui/Widget.cpp
#        skyui/Compositer.cpp
#        skyui/System.cpp
        components/Button.cpp
        skyui/Window.cpp
        skyui/Window.h
        skyui/utils.cpp
        skyui/utils.h
        components/Image.cpp
        components/Image.h
)
