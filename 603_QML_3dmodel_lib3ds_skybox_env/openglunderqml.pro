QT += qml quick

HEADERS += \
    scene.h \
    lib3ds/atmosphere.h \
    lib3ds/background.h \
    lib3ds/camera.h \
    lib3ds/chunk.h \
    lib3ds/chunktable.h \
    lib3ds/ease.h \
    lib3ds/file.h \
    lib3ds/io.h \
    lib3ds/light.h \
    lib3ds/material.h \
    lib3ds/matrix.h \
    lib3ds/mesh.h \
    lib3ds/node.h \
    lib3ds/quat.h \
    lib3ds/shadow.h \
    lib3ds/tcb.h \
    lib3ds/tracks.h \
    lib3ds/types.h \
    lib3ds/vector.h \
    lib3ds/viewport.h \
    3dmodel.h \
    skybox.h
SOURCES += main.cpp \
    scene.cpp \
    lib3ds/atmosphere.c \
    lib3ds/background.c \
    lib3ds/camera.c \
    lib3ds/chunk.c \
    lib3ds/ease.c \
    lib3ds/file.c \
    lib3ds/io.c \
    lib3ds/light.c \
    lib3ds/material.c \
    lib3ds/matrix.c \
    lib3ds/mesh.c \
    lib3ds/node.c \
    lib3ds/quat.c \
    lib3ds/shadow.c \
    lib3ds/tcb.c \
    lib3ds/tracks.c \
    lib3ds/vector.c \
    lib3ds/viewport.c \
    3dmodel.cpp \
    skybox.cpp
RESOURCES += openglunderqml.qrc

CONFIG += c++11

target.path = $$[PWD]/openglunderqml
INSTALLS += target

OTHER_FILES +=
