#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QScreen>
#include "openglwindow.h"
#include "cube.h"
#include "land.h"

/// Vertex shader pouzivany pro zpracovani vrcholu
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

/// Vertex shader pouzivany pro zpracovani barvy pixelu
static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

/// Trida, ktera reprezentuje OpenGL okno.
/// Ta obsahuje zaklad vykreslovani
class SceneWindow : public OpenGLWindow
{
public:
    SceneWindow();
    ~SceneWindow();

    /// Metoda na inicializaci, kterou musime prekryt z predka
    void initialize();
    /// Metoda na vykresleni, kterou musime prekryt z predka
    void render();

private:
    /// Identifikator tranformacni matice, ktera se posida do shaderu
    GLuint m_matrixUniform;
    /// Identifikator atributu pozice vrcholu, ktera se posida do shaderu
    GLuint m_posAttr;
    /// Identifikator atributu barvy vrcholu, ktera se posida do shaderu
    GLuint m_colAttr;

    /// Objekt, ktery je soucasti sceny a bude vykreslovan
    Cube* m_cube;

    Land* m_land;
    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program;
    /// Pomocna promenna s uhlem otoceni
    int m_rotationAngle;
};

#endif // SCENEWINDOW_H
