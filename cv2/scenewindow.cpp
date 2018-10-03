#include "scenewindow.h"

SceneWindow::SceneWindow()
    : m_program(nullptr)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;
}

void SceneWindow::initialize()
{
    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_program->link();

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
}

void SceneWindow::render()
{
    /// Nabindujeme program se shadery
    m_program->bind();

    /// Nastavime vykreslovani na obrazovku
    const qreal retinaScale = devicePixelRatio();
    m_functions->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    /// Vymazeme plochu pro vykreslovani
    m_functions->glClear(GL_COLOR_BUFFER_BIT);

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");

    /// Nachystame data
    GLfloat vertices[] = {
        0.0f, 0.707f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    /// Nastavime ukazatele pro cteni dat
    m_functions->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    m_functions->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    /// a vykreslime objekt
    m_functions->glDrawArrays(GL_TRIANGLES, 0, 3);

    /// Zrusime nastaveni posilanych hodnot
    m_program->disableAttributeArray("posAttr");
    m_program->disableAttributeArray("colAttr");

    /// Uvolnime program
    m_program->release();

    /// Testujeme, zda nenastala nejaka chyba
    GLenum err;
    while ((err = m_functions->glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err << "\n";
    }
}
