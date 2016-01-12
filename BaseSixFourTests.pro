QT       -= core gui

TARGET = BaseSixFourTests

CONFIG -= qt
CONFIG += console
CONFIG += c++11

TEMPLATE = app

SYS_LIB_PATH = /usr/local/lib
SYS_INC_PATH = /usr/local/include

INCLUDEPATH += $${SYS_INC_PATH}
INCLUDEPATH += include/

LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -lgtest -lpthread

HEADERS += include/BaseSixFour.h

SOURCES += tests/BaseSixFourTests.cpp \
           tests/main.cpp
