#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLTexture>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector2D texCoord2;
};

/// Trida, ktera predstavuje urcity elementarni samostatny 3D objekt
class Cube{
private:
    /// Pomocna trida zastupujici shader.
    /// Predal jsem si ji z nadrazene struktury
    QOpenGLShaderProgram *m_program;

    /// jmeno promenne pod kterym vystupuje pozice v shaderu
    std::string m_positionAttributeName;
    /// jmeno promenne pod kterym vystupuji texturovaci souradnice v shaderu
    std::string m_texcoordAttributeName;
    /// jmeno promenne pro druhe tex. souradnice
    std::string m_secondTextureAttributeName;
    /// jmeno uniformni promenne pro primarni texturu
    std::string m_primaryTextureUniformName;
    /// jmeno uniformni promenne pro sekundarni texturu
    std::string m_secondaryTextureUniformName;

    /// Buffer s vrcholy objektu
    QOpenGLBuffer m_vertexBuffer;
    /// Buffer s indexy na vrcholy objektu
    QOpenGLBuffer m_indicesBuffer;

    /// Textura, ktera je aplikovana na objekt
    QOpenGLTexture *m_textureNumbers;
    /// Jina textura, ktera je aplikovana na objekt
    QOpenGLTexture *m_textureBlood;

    /// Metoda pro inicializaci textur
    void initTextures();

public:
    /// Konstruktor pouze nastavi promenne na pocatecni hodnoty
    Cube(GLint size, QOpenGLShaderProgram* program,
         const std::string positionAttributeName,
         const std::string textureAttributeName,
         const std::string secondTextureAttributeName,
         const std::string primaryTextureUniformName,
         const std::string secondaryTextureUniformName);
    /// Provede vykresleni objektu
    void draw();
    /// Zrusi dynamicke soucasti a pripadne uvolni buffery
	~Cube();
};
