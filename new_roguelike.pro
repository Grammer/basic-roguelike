#-------------------------------------------------
#
# Project created by QtCreator 2013-12-21T21:58:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = new_roguelike
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    world.cpp \
    gui.cpp \
    base_character.cpp \
    engine.cpp

HEADERS += \
    gui.h \
    list.h \
    world.h \
    engine.h \
    base_character.h \
    base.h
