#include "land.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Land::Land(GLfloat size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
           const std::string positionAttributeName, const std::string colorAttributeName)
           :m_vertexBuffer(QOpenGLBuffer()), m_indicesBuffer(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
{
    /// Ulozim ukazatele na pomocne tridy
    m_program = program;
    m_functions = functions;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_colorAttributeName = colorAttributeName;
    m_positionAttributeName = positionAttributeName;

    /*
     0,size  size, size
      +------+
      |      |
      |      |
      +------+
     0,0     size, 0
     */

    // vytvorime pole hodnot
    static Vertex1 vertices[] = {
        { {0.0f,      0.0f,      0.0f},{ 0.0, 1.0, 0.0 } },// první jehlan
        { {-1, -1.0f, -1.0f},{ 0.0, 0.4, 0.0 } },// 1
        { {1, -1, -1.0f},{ 0.0, 0.3, 0.0 } },// 2
        { {1,  -1, 1},{ 0.0, 0.2, 0.0 } },// 3
        { {-1, -1,  -1},{ 0.0, 1.0, 0.0 } },// 4

        { {0.0f,      -1.0f,      0.0f},{ 0.0, 1.0, 0.0 } },// první jehlan
        { {-1, -2.0f, -1.0f},{ 0.0, 0.4, 0.0 } },// 1
        { {1, -2, -1.0f},{ 0.0, 0.3, 0.0 } },// 2
        { {1,  -2, 1},{ 0.0, 0.2, 0.0 } },// 3
        { {-1, -2,  -1},{ 0.0, 1.0, 0.0 } },// 4
    };

    GLubyte indices[] = {
        0, 1, 2, 0, 2, 3, 0, 3, 4,
        5, 6, 7, 5, 7, 8, 5, 8, 9
    };

    /// Vytvorim buffer pro vrcholy
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( vertices, sizeof(vertices) );

    /// Vytvorim buffer pro indexy na vrcholy
    m_indicesBuffer.create();
    m_indicesBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_indicesBuffer.bind();
    m_indicesBuffer.allocate( indices, sizeof(indices) );
}

void Land::drawCube(){
    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();
    m_indicesBuffer.bind();

    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, 0, 3, sizeof(Vertex1));
    m_program->enableAttributeArray(m_colorAttributeName.c_str());
    m_program->setAttributeBuffer(m_colorAttributeName.c_str(), GL_FLOAT, 3*sizeof(GL_FLOAT), 3, sizeof(Vertex1));

    /// Provedeme samotne vykresleni
    m_functions->glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0);	// front, top, rear, bottom

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_colorAttributeName.c_str());
}

Land::~Land(){
    m_vertexBuffer.destroy();
    m_indicesBuffer.destroy();
}

