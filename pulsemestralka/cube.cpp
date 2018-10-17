#include "Cube.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Cube::Cube(GLint size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
           const std::string positionAttributeName, const std::string colorAttributeName)
           :m_vertexBuffer(QOpenGLBuffer()), m_indicesBuffer(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
{
    /// Ulozim ukazatele na pomocne tridy
    m_program = program;
    m_functions = functions;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_colorAttributeName = colorAttributeName;
    m_positionAttributeName = positionAttributeName;
	
    // vytvorime pole hodnot
    static Vertex vertices[] = {
        // cervene
        { {-10.0f, 0.0f, 0.0f},{ 1.0, 0.0, 0.0 } },// 0
        { {-11.0f, -1.0f, 0.0f},{ 1.0, 0.0, 0.0 } },// 1
        { {-9.0f, -1, 0.0f},{ 1.0, 0.0, 0.0 } },// 2

        //sede

        { {-10.0f, 0.0f, 0.0f},{ 0.5, 0.5, 0.5 } },// 3
        { {-11.0f, -1.0f, 0.0f},{ 0.5, 0.5, 0.5 } },// 4
        { {-9.0f, -1, 0.0f},{ 0.5, 0.5, 0.5 } },// 5

        //modre

        { {-10.0f, -1.0f, -4.0f},{ 0.0, 0.0, 1.0 } },// 6
    };

    GLubyte indices[] = {
        2, 1, 0,
        3, 4, 6,
        3, 5, 6,
        6, 4, 5
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

void Cube::drawCube(){	
    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();
    m_indicesBuffer.bind();

    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, 0, 3, sizeof(Vertex));
    m_program->enableAttributeArray(m_colorAttributeName.c_str());
    m_program->setAttributeBuffer(m_colorAttributeName.c_str(), GL_FLOAT, 3*sizeof(GL_FLOAT), 3, sizeof(Vertex));


    ///m_functions->glCullFace(GL_BACK);
    /// Provedeme samotne vykresleni
    m_functions->glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, 0);


    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_colorAttributeName.c_str());
}

Cube::~Cube(){
    m_vertexBuffer.destroy();
    m_indicesBuffer.destroy();
}

