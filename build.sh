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
  skyui/Pen.cpp \
  skyui/Painter.cpp \
  skyui/Widget.cpp \
  skyui/Window.cpp \
  components/Button.cpp \
  -Iskyui \
  -Icomponents \
  `pkg-config --libs --cflags Magick++ x11 cairo freetype2`
