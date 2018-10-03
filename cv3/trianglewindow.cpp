#include "trianglewindow.h"

TriangleWindow::TriangleWindow()
    : m_program(0)
{
}


void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");

    // vygenerovani cisel pro ID bufferu
    glGenBuffers(1, &m_vertexBufferId);
    glGenBuffers(1, &m_colorBufferId);

    // definice vrcholu a barev
    GLfloat vertices[] = {
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
    };

    GLfloat colors[] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };

    // propojeni jmena bufferu a jeho typu
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    // vytvoreni bufferu a naplneni polem vrcholu
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferId);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    // vykresleni 3 vrcholu trojuhelniku
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);

    m_program->release();
}
