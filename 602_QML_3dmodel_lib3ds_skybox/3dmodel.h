#include <QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include "lib3ds/file.h"
#include "lib3ds/mesh.h"
#include <cassert>

class CModel3DS
{
public:
    CModel3DS(std:: string filename,
              QOpenGLShaderProgram* program,
              const std::string positionAttributeName,
              const std::string textureAttributeName);
    /// Metoda pro vykresleni objektu
    virtual void Draw() const;
    /// Metoda, kterou musite rucne zavola pro vytvoreni VBO
    virtual void CreateVBO();
    virtual ~CModel3DS();
protected:
    /// Pomocna metoda, ktera spocte pocet projuhelniku meshe
    void GetFaces();
    /// Pocet trojuhelniku tvoricich mesh objektu
    unsigned int m_TotalFaces;
    /// Model, ktery je nacten ze souboru
    Lib3dsFile * m_model;
    /// identifikator VBO s vrcholy
    GLuint m_VertexVBO;
    /// identifikator VBO s normalami
    GLuint m_NormalVBO;

    /// Shader program predany z nadrazene struktury
    QOpenGLShaderProgram *m_program;

    /// jmeno promenne pod kterym vystupuje pozice v shaderu
    std::string m_positionAttributeName;
    /// jmeno promenne pod kterym vystupuji normaly
    std::string m_normalAttributeName;
};


