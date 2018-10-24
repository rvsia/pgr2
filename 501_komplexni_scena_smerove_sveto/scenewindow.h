#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QVector>
#include "openglwindow.h"
#include "cube.h"

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
    /// Identifikator projekcni matice, ktera se posila do shaderu
    GLuint m_projectionMatrixUniform;
    /// Identifikator pohledove matice, ktera se posila do shaderu
    GLuint m_modelViewMatrixUniform;
    /// Identifikator atributu pozice vrcholu, ktera se posida do shaderu
    GLuint m_positionAttr;
    /// Identifikator atributu barvy vrcholu, ktera se posida do shaderu
    GLuint m_normalAttr;

    /// Sezna, vsech objektu, ktere budou ve scene nachazet
    QVector<GraphicalObject*> m_sceneContent;

    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program;
    /// Pomocna promenna s uhlem otoceni
    int m_rotationAngle;
};

#endif // SCENEWINDOW_H
