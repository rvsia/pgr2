include(openglwindow.pri)

SOURCES += \
    main.cpp \
    graphicalobject.cpp \
    car.cpp

target.path = $$[PWD]/openglwindow
INSTALLS += target

CONFIG += c++11

HEADERS += \
    graphicalobject.h \
    car.h
