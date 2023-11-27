#!/bin/bash

[[ ! -d "build" ]] && mkdir build

g++ -o build/skyuidemo \
  main.cpp \
  skyui/Application.cpp \
  skyui/Pixmap.cpp \
  skyui/Color.cpp \
  skyui/FontEngine.cpp \
  skyui/ImageEngine.cpp \
  skyui/Pen.cpp \
  skyui/Painter.cpp \
  skyui/Window.cpp \
  skyui/Compositer.cpp \
  skyui/View.cpp \
  -Iskyui -Iuidecl \
  `pkg-config --libs --cflags Magick++ x11 cairo freetype2`
