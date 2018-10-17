#include "3dmodel.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Model3d::Model3d(std::string file, QOpenGLShaderProgram* program, QOpenGLFunctions* functions,
           const std::string positionAttributeName, const std::string textureAttributeName)
           :m_vertexBuffer(QOpenGLBuffer()), m_indicesBuffer(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
{
    /// Ulozim ukazatele na pomocne tridy
    m_program = program;
    m_functions = functions;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_texcoordAttributeName = textureAttributeName;
    m_positionAttributeName = positionAttributeName;


    /// Zavolani nacteni 3d modelu ze souboru
    if (Load3DS (&object, file.c_str()) != 1)
        qDebug() << "Cannot open file \n";

    /// Vytvorim buffer pro vrcholy
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( object.vertex, object.vertices_qty*sizeof(vertex_type) ); //!

    /// Vytvorim buffer pro texturovaci souradnice
    m_textureBuffer.create();
    m_textureBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_textureBuffer.bind();
    m_textureBuffer.allocate( object.mapcoord, object.vertices_qty*sizeof(mapcoord_type) ); //!

    /// Vytvorim buffer pro indexy na vrcholy
    m_indicesBuffer.create();
    m_indicesBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indicesBuffer.bind();
    m_indicesBuffer.allocate( object.indices, object.polygons_qty*3*sizeof(GLuint) );
}

void Model3d::draw(){
    /// Vytvorime pointery, ktere budou posilat data do shaderu. Aktivace byla udelana o uroven vys
    /// Existuje vyce variant jak tento krok udelat - glVertexAttribPointer, setAttributeBuffer atp.
    /// Je to jedno, ktery pouzijeme

    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();
    int offset = 0;
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, offset, 3, sizeof(vertex_type));

    m_textureBuffer.bind();
    offset = 0;
    m_program->enableAttributeArray(m_texcoordAttributeName.c_str());
    m_program->setAttributeBuffer(m_texcoordAttributeName.c_str(), GL_FLOAT, offset, 2, sizeof(mapcoord_type));

    m_indicesBuffer.bind();

    /// Provedeme samotne vykresleni
    m_functions->glDrawElements(GL_TRIANGLES, object.polygons_qty*3, GL_UNSIGNED_INT, 0);

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
    m_program->disableAttributeArray(m_texcoordAttributeName.c_str());
}

Model3d::~Model3d(){
    m_vertexBuffer.destroy();
    m_indicesBuffer.destroy();
    m_textureBuffer.destroy();
}

