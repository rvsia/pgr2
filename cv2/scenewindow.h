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
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   vec4 tmpPos = posAttr;\n"
    "   tmpPos.x -= 0.3;\n"
    "   gl_Position =  tmpPos;\n"
    "}\n";

/// Fragment shader pouzivany pro zpracovani barvy pixelu
static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
//    "   vec4 tmpColor = col;"
//    "   tmpColor.r = 0.0; "
    "   gl_FragColor = vec4(1.0 - col.r,1.0 - col.g,1.0 - col.b,1);\n"
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
    /// Identifikator atributu pozice vrcholu, ktera se posida do shaderu
    GLuint m_posAttr;
    /// Identifikator atributu barvy vrcholu, ktera se posida do shaderu
    GLuint m_colAttr;

    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program;
};

#endif // SCENEWINDOW_H
