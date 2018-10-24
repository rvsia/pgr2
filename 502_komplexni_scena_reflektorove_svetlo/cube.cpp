#include "Cube.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Cube::Cube(GLfloat size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
           const std::string positionAttributeName, const std::string colorAttributeName):
    GraphicalObject(program, functions, positionAttributeName, colorAttributeName),
    m_vertexBuffer(QOpenGLBuffer())
{
    // vytvorime pole hodnot
    Vertex vertices[] = {
        { {0.0f-size, 0.0f-size, 0.0f-size}, {1.0f, 0.0f, 0.0}, { 0.0f, 0.0f,-1.0f}, {0.0f, 0.0}},// 0
        { {0.0f+size, 0.0f-size, 0.0f-size}, {0.0f, 1.0f, 0.0}, { 0.0f, 0.0f,-1.0f}, {1.0f, 0.0}},// 1
        { {0.0f-size, 0.0f+size, 0.0f-size}, {1.0f, 1.0f, 0.0}, { 0.0f, 0.0f,-1.0f}, {1.0f, 1.0}},// 4
        { {0.0f+size, 0.0f+size, 0.0f-size}, {0.0f, 0.0f, 1.0}, { 0.0f, 0.0f,-1.0f}, {0.0f, 1.0}},// 5

        { {0.0f+size, 0.0f-size, 0.0f+size}, {0.0f, 1.0f, 0.0}, { 1.0f, 0.0f, 0.0f}, {0.0f, 0.0}},// 1
        { {0.0f+size, 0.0f-size, 0.0f-size}, {0.0f, 1.0f, 0.0}, { 1.0f, 0.0f, 0.0f}, {1.0f, 0.0}},// 2
        { {0.0f+size, 0.0f+size, 0.0f+size}, {0.0f, 0.0f, 1.0}, { 1.0f, 0.0f, 0.0f}, {1.0f, 1.0}},// 5
        { {0.0f+size, 0.0f+size, 0.0f-size}, {0.0f, 0.0f, 1.0}, { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0}},// 6

        { {0.0f-size, 0.0f-size, 0.0f+size}, {0.0f, 1.0f, 0.0}, { 0.0f, 0.0f, 1.0f}, {0.0f, 0.0}},// 2
        { {0.0f+size, 0.0f-size, 0.0f+size}, {1.0f, 0.0f, 0.0}, { 0.0f, 0.0f, 1.0f}, {1.0f, 0.0}},// 3
        { {0.0f-size, 0.0f+size, 0.0f+size}, {0.0f, 0.0f, 1.0}, { 0.0f, 0.0f, 1.0f}, {1.0f, 1.0}},// 6
        { {0.0f+size, 0.0f+size, 0.0f+size}, {1.0f, 1.0f, 0.0}, { 0.0f, 0.0f, 1.0f}, {0.0f, 1.0}},// 7

        { {0.0f-size, 0.0f-size, 0.0f+size}, {1.0f, 0.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0}},// 3
        { {0.0f-size, 0.0f-size, 0.0f-size}, {1.0f, 0.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0}},// 0
        { {0.0f-size, 0.0f+size, 0.0f+size}, {1.0f, 1.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0}},// 7
        { {0.0f-size, 0.0f+size, 0.0f-size}, {1.0f, 1.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0}} // 4
    };

    /// Vytvorim buffer pro vrcholy
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( vertices, sizeof(vertices) );
}

void Cube::draw(){
    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();

    /// Vytvorime pointery, ktere budou posilat data do shaderu. Aktivace byla udelana o uroven vys
    /// Existuje vyce variant jak tento krok udelat - glVertexAttribPointer, setAttributeBuffer atp.
    /// Je to jedno, ktery pouzijeme
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, 0, 3, sizeof(Vertex));
    m_program->enableAttributeArray(m_normalAttributeName.c_str());
    m_program->setAttributeBuffer(m_normalAttributeName.c_str(), GL_FLOAT, 6*sizeof(GL_FLOAT), 3, sizeof(Vertex));

    /// Provedeme samotne vykresleni
    m_functions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_functions->glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
    m_functions->glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    m_functions->glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_normalAttributeName.c_str());
}

Cube::~Cube(){
    m_vertexBuffer.destroy();
}

