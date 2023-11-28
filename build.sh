#!/bin/bash

[[ ! -d "build" ]] && mkdir build

g++ -o build/skyuidemo \
  main.cpp \
  skyui/Application.cpp \
  skyui/Pixmap.cpp \
  skyui/Color.cpp \
  skyui/Compositer.cpp \
  skyui/Event.cpp \
  skyui/FontEngine.cpp \
  skyui/ImageEngine.cpp \
  skyui/Pen.cpp \
  skyui/Painter.cpp \
  skyui/Window.cpp \
  skyui/View.cpp \
  -Iskyui \
  `pkg-config --libs --cflags Magick++ x11 cairo freetype2`
