include(openglwindow.pri)

CONFIG += c++11

SOURCES += \
    main.cpp \
    land.cpp

target.path = $$[PWD]/openglwindow
INSTALLS += target

HEADERS += \
    land.h
