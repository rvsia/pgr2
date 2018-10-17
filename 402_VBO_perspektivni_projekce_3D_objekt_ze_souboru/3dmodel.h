#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QDir>

#include "declarations.h"
#include "3dsloader.h"

/// Trida, ktera predstavuje urcity elementarni samostatny 3D objekt
class Model3d{
private:
    /// Pomocna trida zastupujici shader.
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLShaderProgram *m_program;
    /// Pomocna trida zastresujici vsechny OpenGL funkce
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLFunctions* m_functions;

    /// jmeno promenne pod kterym vystupuje pozice v shaderu
    std::string m_positionAttributeName;
    /// jmeno promenne pod kterym vystupuji texturovaci souradnice v shaderu
    std::string m_texcoordAttributeName;

    /// Buffer s vrcholy objektu
    QOpenGLBuffer m_vertexBuffer;
    /// Buffer s indexy na vrcholy objektu
    QOpenGLBuffer m_indicesBuffer;
    /// Buffer s texturovacimi souradnicemi vrcholu objektu
    QOpenGLBuffer m_textureBuffer;

    obj_type object;

public:
    /// Konstruktor pouze nastavi promenne na pocatecni hodnoty
    Model3d(std::string path, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
         const std::string positionAttributeName, const std::string textureAttributeName);
    /// Provede vykresleni objektu
    void draw();
    /// Zrusi dynamicke soucasti a pripadne uvolni buffery
    ~Model3d();
};
