#include "skybox.h"

/// Promenna m_indicesBuffer je inicializovana staticky, protoze se jedna hodnotu, ne o
/// ukazatel. Mohli bychom to predelat na ukazatel, ale pak bychom se museli starat o
/// uvolneni objektu. Stejne tak m_vertexBuffer.
Skybox::Skybox(GLfloat size, QOpenGLShaderProgram* program,
               const std::string positionAttributeName,
               const std::string textureUniformName,
               std::array<std::string, 6> textureFiles)
           :m_vertexBuffer(QOpenGLBuffer())
{
    /// Ulozim ukazatele na pomocne tridy
    m_program = program;

    /// Ulozim jmena pod kterymi se posilaji promenne do shaderu
    m_positionAttributeName = positionAttributeName;
    m_textureUniformName = textureUniformName;
    m_textureFiles = textureFiles;
	
    /// vytvorime pole hodnot
    GLfloat points[] = {
      -size,  size, -size,
      -size, -size, -size,
       size, -size, -size,
       size, -size, -size,
       size,  size, -size,
      -size,  size, -size,

      -size, -size,  size,
      -size, -size, -size,
      -size,  size, -size,
      -size,  size, -size,
      -size,  size,  size,
      -size, -size,  size,

       size, -size, -size,
       size, -size,  size,
       size,  size,  size,
       size,  size,  size,
       size,  size, -size,
       size, -size, -size,

      -size, -size,  size,
      -size,  size,  size,
       size,  size,  size,
       size,  size,  size,
       size, -size,  size,
      -size, -size,  size,

      -size,  size, -size,
       size,  size, -size,
       size,  size,  size,
       size,  size,  size,
      -size,  size,  size,
      -size,  size, -size,

      -size, -size, -size,
      -size, -size,  size,
       size, -size, -size,
       size, -size, -size,
      -size, -size,  size,
       size, -size,  size
    };

    /// Vytvorim buffer pro vrcholy
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( points, 3 * 36 * sizeof (GLfloat) );

    /// Inicializuj texturovaci objekt
    initTextures();
}

void Skybox::initTextures()
{
    glActiveTexture (GL_TEXTURE0);
    glGenTextures(1, &m_textureObjectId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObjectId);
    qDebug() << "Texture id: " << m_textureObjectId << "\n";

    std::array<GLenum, 6> sides = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    /// 6 protoze 6 je sten krychle
    for (unsigned int i = 0 ; i < 6 ; i++) {
        QImage tmpImage(m_textureFiles.at(i).c_str());
        QImage image = tmpImage.convertToFormat(QImage::Format_RGB32);
        glTexImage2D(sides.at(i), 0, GL_RGB, image.width(), image.height(),
                     0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    /*
    m_cubeTexture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    QImage negZImage(":/textures/sky01.png");

    m_cubeTexture->setFormat(QOpenGLTexture::RGB16_UNorm);
    m_cubeTexture->setSize(negZImage.width(), negZImage.height());

    m_cubeTexture->allocateStorage();
    m_cubeTexture->setData(0, 0,
                           QOpenGLTexture::CubeMapNegativeZ,
                           QOpenGLTexture::RGB,
                           QOpenGLTexture::Int16,
                           negZImage.convertToFormat(QImage::Format_RGB16).data_ptr());
                           */
}

void Skybox::draw(){
    /// Nabinduju potrebne buffery
    m_vertexBuffer.bind();

    int offset = 0;
    m_program->enableAttributeArray(m_positionAttributeName.c_str());
    m_program->setAttributeBuffer(m_positionAttributeName.c_str(), GL_FLOAT, offset, 3, 3*sizeof(GLfloat));

    glDepthMask (GL_FALSE);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_CUBE_MAP, m_textureObjectId);
    m_program->setUniformValue(m_textureUniformName.c_str(), 0);
    glDrawArrays (GL_TRIANGLES, 0, 36);
    glDepthMask (GL_TRUE);

    /// Vypneme atributy
    m_program->disableAttributeArray(m_positionAttributeName.c_str());
}

Skybox::~Skybox(){
    m_vertexBuffer.destroy();
    // uvolnit texturu
}

