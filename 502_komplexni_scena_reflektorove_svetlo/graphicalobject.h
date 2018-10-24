#ifndef GRAPHICALOBJECT_H
#define GRAPHICALOBJECT_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

/// Obecny predek vsech objektu, ktere budou vykreslovany
class GraphicalObject
{
public:
    GraphicalObject(QOpenGLShaderProgram* program,
                    QOpenGLFunctions* functions,
                    const std::string positionAttributeName,
                    const std::string colorAttributeName);

    /// Ciste virtualni metoda, ktera slouzi k tomu, aby vynutila implementaci u potomku
    virtual void draw() = 0;

    /// Virtualni destruktor je zde pouze proto, aby se volaly destruktory potomku
    virtual ~GraphicalObject(){}
protected:
    /// Pomocna trida zastupujici shader.
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLShaderProgram *m_program;
    /// Pomocna trida zastresujici vsechny OpenGL funkce
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLFunctions* m_functions;

    /// jmeno promenne pod kterym vystupuje pozice v shaderu
    std::string m_positionAttributeName;
    /// jmeno promenne pod kterym vystupuje normala v shaderu
    std::string m_normalAttributeName;
};

#endif // GRAPHICALOBJECT_H
