#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QScreen>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QBasicTimer>
#include <QOpenGLTexture>

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

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void timerEvent(QTimerEvent *e);

    void initTextures();

private:
    /// Identifikator tranformacni matice, ktera se posida do shaderu
    GLuint m_matrixUniform;
    /// Identifikator atributu pozice vrcholu, ktera se posida do shaderu
    GLuint m_posAttr;
    /// Identifikator atributu textury vrcholu, ktera se posida do shaderu
    GLuint m_texAttr;
    /// Textura, ktera je aplikovana na objekt
    QOpenGLTexture *m_texture;

    /// Objekt, ktery je soucasti sceny a bude vykreslovan
    Cube* m_cube;
    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program;

    /// Pomocna promenna s uhlem otoceni pro automaticky se otacejici kostku
    int m_rotationAngle;

    // -----------------------------------------------------------------
    //                         Atributy pro otoceni
    // -----------------------------------------------------------------
    /// Casovac, ktery se pouziva pro otoceni dotykem
    QBasicTimer m_timer;
    /// Misto, kde bylo zmacknuto tlacitko mysi, ktere inicijuje otoceni
    QVector2D m_mousePressPosition;
    /// Osa okolo ktere se bude otacet
    QVector3D m_rotationAxis;
    /// Rychlost otaceni
    qreal m_angularSpeed;
    /// Struktura skalaru (rychlost otoceni) a vektoru (osa otoceni)
    QQuaternion m_rotation;
};

#endif // SCENEWINDOW_H
