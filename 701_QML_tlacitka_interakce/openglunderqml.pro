QT += qml quick

HEADERS += \
    scene.h \
    cube.h
SOURCES += main.cpp \
    scene.cpp \
    cube.cpp
RESOURCES += openglunderqml.qrc

CONFIG += c++11

target.path = $$[PWD]/openglunderqml
INSTALLS += target

OTHER_FILES +=
