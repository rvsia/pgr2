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
        { {0.0f,      0.0f,      0.0f},{ 0.0, 1.0, 0.0 } },// 0
        { {0.0f+50*size, 0.0f,      0.0f},{ 0.0, 0.4, 0.0 } },// 1
        { {0.0f+50*size, 0.0f-size, 0.0f},{ 0.0, 0.3, 0.0 } },// 2
        { {0.0f,      0.0f-size, 0.0f},{ 0.0, 0.2, 0.0 } },// 3
        { {0.0f,      0.0f,      0.0f+50*size},{ 0.0, 1.0, 0.0 } },// 4
        { {0.0f+50*size, 0.0f,      0.0f+50*size},{ 0.0, 0.5, 0.0 } },// 5
        { {0.0f+50*size, 0.0f-size, 0.0f+50*size},{ 0.0, 1.0, 0.0 } },// 6
        { {0.0f,      0.0f-size, 0.0f+50*size},{ 0.0, 0.4, 0.0 } } // 7
    };

    GLubyte indices[] = {
        0, 1, 3, 2, 7, 6, 4, 5, 0, 1, // front, top, rear, bottom
        1, 5, 2, 6, // right
        0, 4, 3, 7  // left
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

    /**
    // ---- Alternativni reseni kodu nize ----
    // pouzivaji se pouze indexy misto jmen
    // Aktivujeme atributy, ktere se budou posilat
    m_functions->glEnableVertexAttribArray(m_posAttr);
    m_functions->glEnableVertexAttribArray(m_colAttr);
    // vykreslime
    m_functions->glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) 0);
    m_functions->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) (3*sizeof(GL_FLOAT)));
    // Po vykresleni zase atributy deaktivujeme
    m_functions->glDisableVertexAttribArray(m_posAttr);
    m_functions->glDisableVertexAttribArray(m_colAttr);
    **/

    /// Vytvorime pointery, ktere budou posilat data do shaderu. Aktivace byla udelana o uroven vys
    /// Existuje vyce variant jak tento krok udelat - glVertexAttribPointer, setAttributeBuffer atp.
    /// Je to jedno, ktery pouzijeme
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, 0, 3, sizeof(Vertex1));
    m_program->enableAttributeArray(m_colorAttributeName.c_str());
    m_program->setAttributeBuffer(m_colorAttributeName.c_str(), GL_FLOAT, 3*sizeof(GL_FLOAT), 3, sizeof(Vertex1));

    /// Provedeme samotne vykresleni
    m_functions->glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_BYTE, 0);	// front, top, rear, bottom
    m_functions->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (void*)(10*sizeof(GLubyte)) ); // right
    m_functions->glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (void*)(14*sizeof(GLubyte)) ); // left

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_colorAttributeName.c_str());
}

Land::~Land(){
    m_vertexBuffer.destroy();
    m_indicesBuffer.destroy();
}

