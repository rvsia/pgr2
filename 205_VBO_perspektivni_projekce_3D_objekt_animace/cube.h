#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

struct Vertex{
    GLfloat position[3];
    GLfloat color[3];
};

/// Trida, ktera predstavuje urcity elementarni samostatny 3D objekt
class Cube{
private:
    /// Pomocna trida zastupujici shader.
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLShaderProgram *m_program;
    /// Pomocna trida zastresujici vsechny OpenGL funkce
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLFunctions* m_functions;

    /// jmeno promenne pod kterym vystupuje pozice v shaderu
    std::string m_positionAttributeName;
    /// jmeno promenne pod kterym vystupuje barva v shaderu
    std::string m_colorAttributeName;

    /// Buffer s vrcholy objektu
    QOpenGLBuffer m_vertexBuffer;
    /// Buffer s indexy na vrcholy objektu
    QOpenGLBuffer m_indicesBuffer;
public:
    /// Konstruktor pouze nastavi promenne na pocatecni hodnoty
    Cube(GLint size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
         const std::string positionAttributeName, const std::string colorAttributeName);
    /// Provede vykresleni objektu
    void drawCube();
    /// Zrusi dynamicke soucasti a pripadne uvolni buffery
	~Cube();
};


#endif // SCENEWINDOW_H
