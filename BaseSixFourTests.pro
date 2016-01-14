QT       -= core gui

TARGET = BaseSixFourTests

CONFIG -= qt
CONFIG += console
CONFIG += c++11

TEMPLATE = app

INCLUDEPATH += /usr/local/include \
               include/

LIBS += -L/usr/lib/x86_64-linux-gnu
LIBS += -lgtest -lpthread

HEADERS += include/BaseSixFour.h

SOURCES +=  src/BaseSixFour.cpp \
            tests/BaseSixFourTests.cpp \
            tests/main.cpp
