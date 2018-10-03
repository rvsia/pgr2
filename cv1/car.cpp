#include "car.h"

Car::Car()
{

}

void Car::draw(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    static GLfloat nakladak[] = {
        0.0, -0.250, 1.0, 0.0, 0.0, // 2 sour. vrch., 3 bar. slozky
        0.0, -0.5, 1.0, 0.0, 0.0,
        0.0, -0.5, 1.0, 0.0, 0.0,
        0.25, -0.50, 1.0, 0.0, 0.0,
        0.5, -0.5, 1.0, 0.0, 0.0,
        0.50, 0.0, 1.0, 0.0, 0.0,
        0.25, 0.0, 1.0, 0.0, 0.0,

        0.0, -0.250, 0.0, 0.0, 0.9, // 2 sour. vrch., 3 bar. slozky
        0.25, -0.250, 0.0, 0.0, 0.9,
        0.1, 0.0, 0.0, 0.0, 0.9,
        0.25, 0.0, 0.0, 0.0, 0.9,
    };

    glVertexPointer(2, GL_FLOAT, 5*sizeof(GL_FLOAT), &nakladak[0]);
    glColorPointer(3, GL_FLOAT, 5*sizeof(GL_FLOAT), &nakladak[2]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
    glDrawArrays(GL_TRIANGLE_STRIP, 7, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
