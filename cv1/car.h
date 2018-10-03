#ifndef CAR_H
#define CAR_H

#include "graphicalobject.h"
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLBuffer>

class Car:public GraphicalObject

{
public:
    Car();
    void draw();
};

#endif // CAR_H
