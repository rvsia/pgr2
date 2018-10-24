#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

struct VertexData1
{
    QVector3D position;
    QVector2D texCoord;
};

/// Trida, ktera predstavuje urcity elementarni samostatny 3D objekt
class Stihacka{
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
public:
    /// Konstruktor pouze nastavi promenne na pocatecni hodnoty
    Stihacka(GLint size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
         const std::string positionAttributeName, const std::string textureAttributeName);
    /// Provede vykresleni objektu
    void draw();
    /// Zrusi dynamicke soucasti a pripadne uvolni buffery
    ~Stihacka();
};
