#include "stihacka.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Stihacka::Stihacka(GLint size, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
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
    VertexData1 vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1.0f, 1.0f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.0f, 1.0f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(1.0f, 1.0f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
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

void Stihacka::draw(){
    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();
    m_indicesBuffer.bind();

    /// Vytvorime pointery, ktere budou posilat data do shaderu. Aktivace byla udelana o uroven vys
    /// Existuje vyce variant jak tento krok udelat - glVertexAttribPointer, setAttributeBuffer atp.
    /// Je to jedno, ktery pouzijeme

    int offset = 0;
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, offset, 3, sizeof(VertexData1));

    offset = sizeof(QVector3D);
    m_program->enableAttributeArray(m_texcoordAttributeName.c_str());
    m_program->setAttributeBuffer(m_texcoordAttributeName.c_str(), GL_FLOAT, offset, 2, sizeof(VertexData1));

    /// Provedeme samotne vykresleni
    m_functions->glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_texcoordAttributeName.c_str());
}

Stihacka::~Stihacka(){
    m_vertexBuffer.destroy();
    m_indicesBuffer.destroy();
}

