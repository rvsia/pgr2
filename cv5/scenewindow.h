#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include "openglwindow.h"

/// Vertex shader pouzivany pro zpracovani vrcholu
static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute lowp float movementAttr;\n"
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position =  posAttr;\n"
    "}\n";

static const char *stromecekShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "attribute lowp float movementAttr;\n"
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   vec4 tmpPosition = posAttr;\n"
    "   tmpPosition.x = tmpPosition.x + movementAttr;\n"
    "   tmpPosition.y = tmpPosition.y + movementAttr;\n"
    "   gl_Position =  tmpPosition;\n"
    "}\n";

/// Vertex shader pouzivany pro zpracovani barvy pixelu
static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

struct Vertex{
    GLfloat position[2];
    GLfloat color[3];
};

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
    /// Identifikator atributu pozice vrcholu, ktera se posida do shaderu
    GLuint m_posAttr;
    /// Identifikator atributu barvy vrcholu, ktera se posida do shaderu
    GLuint m_colAttr;

    GLuint m_movementAttr;

    /// Buffer pro trojuhelnik
    QOpenGLBuffer m_stromBuffer;
    /// BUffer pro ctverec
    QOpenGLBuffer m_raketkaBuffer;

    /// BUffer pro ctverec
    QOpenGLBuffer m_posunBuffer;

    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program;

    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program1;
};

#endif // SCENEWINDOW_H
