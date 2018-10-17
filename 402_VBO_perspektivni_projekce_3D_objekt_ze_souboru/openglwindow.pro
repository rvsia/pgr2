include(openglwindow.pri)

CONFIG += c++11

SOURCES += \
    main.cpp

target.path = $$[PWD]/openglwindow
INSTALLS += target

RESOURCES += \
    shaders.qrc \
    models.qrc

OTHER_FILES +=
