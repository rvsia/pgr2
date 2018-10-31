#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLTexture>
#include <array>
#include <QDebug>

struct VertexData
{
    QVector3D position;
};

/// Trida, ktera predstavuje urcity elementarni samostatny 3D objekt
class Skybox{
private:
    /// Pomocna trida zastupujici shader.
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLShaderProgram *m_program;
    /// jmeno promenne pod kterym vystupuje pozice v shaderu
    std::string m_positionAttributeName;
    /// jmeno uniformni promenne pro primarni texturu
    std::string m_textureUniformName;

    /// Buffer s vrcholy objektu
    QOpenGLBuffer m_vertexBuffer;

    /// Textura, ktera je aplikovana na objekt
    GLuint m_textureObjectId;

    /// Pole s nazvy souboru jednotlivych textur
    std::array<std::string, 6> m_textureFiles;

    /// Metoda pro inicializaci textur
    void initTextures();

public:
    /// Konstruktor pouze nastavi promenne na pocatecni hodnoty
    Skybox(GLfloat size, QOpenGLShaderProgram* program,
         const std::string positionAttributeName,
         const std::string textureUniformName,
         std::array<std::string, 6> textureFiles);
    /// Provede vykresleni objektu
    void draw();
    /// Zrusi dynamicke soucasti a pripadne uvolni buffery
    ~Skybox();
};
