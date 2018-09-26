#include "scenewindow.h"

SceneWindow::SceneWindow()
{
}

SceneWindow::~SceneWindow(){
}

void SceneWindow::initialize()
{

}

void SceneWindow::render()
{
    glClear (GL_COLOR_BUFFER_BIT);
    static GLfloat triangle[] = {
        0.0, 0.0, 0.0, 1.0, 0.0, // 2 sour. vrch., 3 bar. slozky
        0.0, 0.5, 0.0, 1.0, 0.0,
        0.5, 0.5, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        0.5, 0.0, 0.0, 0.0, 1.0,
        0.5, 0.5, 0.0, 0.0, 1.0,
        0.0, 0.5, 1.0, 0.0, 0.0,
        0.5, 0.5, 1.0, 0.0, 0.0,
        0.25, 1.0, 1.0, 0.0, 0.0
    };

    static GLfloat triangle1[] = {
        -0.25, 0.0, 0.0, 0.0, 1.0, // 2 sour. vrch., 3 bar. slozky
        0.0, -0.25, 0.0, 0.0, 1.0,
        -0.5, -0.25, 0.0, 0.0, 1.0,
        0.0, -0.750, 0.0, 0.0, 1.0,
        -0.5, -0.750, 0.0, 0.0, 1.0,
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 5*sizeof(GL_FLOAT), &triangle[0]);
    glColorPointer(3, GL_FLOAT, 5*sizeof(GL_FLOAT), &triangle[2]);

    glDrawArrays(GL_TRIANGLES, 0, 9);

    glVertexPointer(2, GL_FLOAT, 5*sizeof(GL_FLOAT), &triangle1[0]);
    glColorPointer(3, GL_FLOAT, 5*sizeof(GL_FLOAT), &triangle1[2]);

    GLubyte indices[] = {0,1,2,1,2,4,1,3,4};

    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glFlush();

    /// Testujeme, zda nenastala nejaka chyba
    GLenum err;
    while ((err = m_functions->glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err << "\n";
    }
}
