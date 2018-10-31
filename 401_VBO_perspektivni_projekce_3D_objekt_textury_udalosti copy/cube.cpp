#include "Cube.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Cube::Cube(GLint size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
           const std::string positionAttributeName, const std::string textureAttributeName)
           :m_vertexBuffer(QOpenGLBuffer()), m_indicesBuffer(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
{
    /// Ulozim ukazatele na pomocne tridy
    m_program = program;
    m_functions = functions;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_texcoordAttributeName = textureAttributeName;
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
    VertexData vertices[] = {
        {QVector3D(-100.0f, -1.0f, -100.0f), QVector2D(0.0f, 0.0f)}, // v16
        {QVector3D( 100.0f, -1.0f, -100.0f), QVector2D(100.0f, 0.0f)}, // v17
        {QVector3D(-100.0f, -1.0f,  100.0f), QVector2D(0.0f, 100.0f)}, // v18
        {QVector3D( 100.0f, -1.0f,  100.0f), QVector2D(100.0f, 100.0f)}, // v19
    };

    GLushort indices[] = {
        0, 0, 1, 2, 3, 3, // Face 4 - triangle strip (v16, v17, v18, v19)
    };

    /// Vytvorim buffer pro vrcholy
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( vertices, sizeof(vertices) );

    /// Vytvorim buffer pro indexy na vrcholy
    m_indicesBuffer.create();
    m_indicesBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indicesBuffer.bind();
    m_indicesBuffer.allocate( indices, sizeof(indices) );
}

void Cube::draw(){
    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();
    m_indicesBuffer.bind();

    /// Vytvorime pointery, ktere budou posilat data do shaderu. Aktivace byla udelana o uroven vys
    /// Existuje vyce variant jak tento krok udelat - glVertexAttribPointer, setAttributeBuffer atp.
    /// Je to jedno, ktery pouzijeme

    int offset = 0;
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, offset, 3, sizeof(VertexData));

    offset = sizeof(QVector3D);
    m_program->enableAttributeArray(m_texcoordAttributeName.c_str());
    m_program->setAttributeBuffer(m_texcoordAttributeName.c_str(), GL_FLOAT, offset, 2, sizeof(VertexData));

    /// Provedeme samotne vykresleni
    m_functions->glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_texcoordAttributeName.c_str());
}

Cube::~Cube(){
    m_vertexBuffer.destroy();
    m_indicesBuffer.destroy();
}

