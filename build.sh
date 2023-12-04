#!/bin/bash

[[ ! -d "build" ]] && mkdir build

g++ -o build/skyuidemo \
  main.cpp \
  skyui/Application.cpp \
  skyui/Pixmap.cpp \
  skyui/Color.cpp \
  skyui/Event.cpp \
  skyui/Font.cpp \
  skyui/Painter.cpp \
  skyui/Widget.cpp \
  skyui/Window.cpp \
  skyui/utils.cpp \
  components/Button.cpp \
  components/Image.cpp \
  -Iskyui \
  -Icomponents \
  `pkg-config --libs --cflags Magick++ x11 cairo freetype2`
