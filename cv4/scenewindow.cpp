#include "scenewindow.h"

SceneWindow::SceneWindow()
    : m_program(nullptr)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;
    m_colorBuffer.destroy();
    m_vertexBuffer.destroy();
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

    /// Vytvorime pole hodnot pro objekt
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

    /// a vytvorime buffer a naplnime jej daty
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( vertices, sizeof(vertices) );

    m_colorBuffer.create();
    m_colorBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_colorBuffer.bind();
    m_colorBuffer.allocate( colors, sizeof(colors) );
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

    /// Nastavime ukazatele pro cteni dat
    m_vertexBuffer.bind();
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 2, 0);
    m_colorBuffer.bind();
    m_program->setAttributeBuffer("colAttr", GL_FLOAT, 0, 3, 0);
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
