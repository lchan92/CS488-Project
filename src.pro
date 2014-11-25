######################################################################
# Automatically generated by qmake (3.0) Wed May 28 16:55:50 2014
######################################################################

QT += opengl widgets
CONFIG += c++11
QMAKE_CXXFLAGS += -W -Wall -g 
LIBS += -lGLU -llua5.1 -lSDL -lSDL_mixer
TEMPLATE = app
TARGET = game
INCLUDEPATH += /usr/include/lua5.1

# Input
HEADERS += algebra.hpp \
           material.hpp \
           primitive.hpp \
           scene.hpp \
           AppWindow.hpp \
           Viewer.hpp \
           character.hpp \
           obstacle_map.hpp \
           sounds.hpp \
           textures.hpp \
           scene_lua.hpp \
           lua488.hpp
SOURCES += algebra.cpp \
           main.cpp \
           material.cpp \
           primitive.cpp \
           scene.cpp \
           AppWindow.cpp \ 
           Viewer.cpp \
           character.cpp \
           sounds.cpp \
           textures.cpp \
           obstacle_map.cpp \
           scene_lua.cpp