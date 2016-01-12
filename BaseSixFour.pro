QT       -= core gui

TARGET = BaseSixFour
TEMPLATE = lib

HEADERS += include/*.h

SOURCES += src/*.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
