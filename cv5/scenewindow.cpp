#include "scenewindow.h"

SceneWindow::SceneWindow()
    : m_program(nullptr)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;
    m_raketkaBuffer.destroy();
    m_stromBuffer.destroy();
    m_posunBuffer.destroy();
}

void SceneWindow::initialize()
{
    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, stromecekShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_program->link();
    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_movementAttr = m_program->attributeLocation("movementAttr");

    /// Vytvorime si pomocne pole s vrcholy
    Vertex raketka[] = {
        { {0.0, 0.0}, {0.0, 0.0, 1.0} },
        { {0.2, 0.0}, {0.0, 0.0, 1.0} },
        { {0.2, 0.4}, {0.0, 0.0, 1.0} },

        { {0.0, 0.0}, {0.0, 0.0, 1.0} },
        { {0.0, 0.4}, {0.0, 0.0, 1.0} },
        { {0.2, 0.4}, {0.0, 0.0, 1.0} },

        { {0.0, 0.4}, {1.0, 0.0, 0.0} },
        { {0.2, 0.4}, {1.0, 0.0, 0.0} },
        { {0.1, 0.6}, {1.0, 0.0, 0.0} },

        { {0.0, 0.0}, {0.0, 0.0, 1.0} },
        { {-0.05, -0.1}, {0.0, 0.0, 1.0} },
        { {0.2, 0.0}, {0.0, 0.0, 1.0} },

        { {0.2, 0.0}, {0.0, 0.0, 1.0} },
        { {0.25, -0.1}, {0.0, 0.0, 1.0} },
        { {-0.05, -0.1}, {0.0, 0.0, 1.0} },
    };

    /// Vytvorime buffer a ulozime do nej pole
    m_raketkaBuffer.create();
    m_raketkaBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_raketkaBuffer.bind();
    m_raketkaBuffer.allocate( raketka, sizeof(raketka) );

    /// vytvorime jine pole hodnot pro druhy objekt
    Vertex strom[] = {
        { { 0.0, 0.0}, {0.0, 1.0, 0.0}},
        { { 0.2, 0.0}, {0.0, 1.0, 0.0}},
        { { 0.1, 0.2}, {0.0, 1.0, 0.0}},

        { { 0.0, -0.1}, {0.0, 1.0, 0.0}},
        { { 0.2, -0.1}, {0.0, 1.0, 0.0}},
        { { 0.1, 0.1}, {0.0, 1.0, 0.0}},

        { { 0.0, -0.2}, {0.0, 1.0, 0.0}},
        { { 0.2, -0.2}, {0.0, 1.0, 0.0}},
        { { 0.1, -0.0}, {0.0, 1.0, 0.0}},
    };

    /// a opet pro nej vytvorime buffer a naplnime jej daty
    m_stromBuffer.create();
    m_stromBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_stromBuffer.bind();
    m_stromBuffer.allocate( strom, sizeof(strom) );

    /// vytvorime jine pole hodnot pro druhy objekt
    GLfloat posuny[] = {
        0.2, 0.2, 0.2,
        0.3, 0.3, 0.3,
        0.4, 0.4, 0.5,

        0.6, 0.6, 0.6,
        0.7, 0.7, 0.7,
        0.1, 0.1, 0.1,

        0.5, 0.5, 0.5,
        -0.2, -0.2, -0.2,
        -0.4, -0.4, -0.4,
    };

    /// a opet pro nej vytvorime buffer a naplnime jej daty
    m_posunBuffer.create();
    m_posunBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_posunBuffer.bind();
    m_posunBuffer.allocate( posuny, sizeof(posuny) );
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

    /// Nabindujeme buffer, nastavime ukazatel a vykreslime
    m_raketkaBuffer.bind();

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");
    m_program->enableAttributeArray("movementAttr");

    /// Nastavime ukazatele pro cteni dat
                                    //name, type, offset, tupleSize, stride
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 2, sizeof(Vertex));
    m_program->setAttributeBuffer("colAttr", GL_FLOAT, 2*sizeof(GL_FLOAT), 3, sizeof(Vertex));

    /// a provedeme vykresleni jednoho objektu
    m_functions->glDrawArrays(GL_TRIANGLES, 0, 15);

    /// a vykreslime druhy objekt


    m_stromBuffer.bind();
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 2, sizeof(Vertex));
    m_program->setAttributeBuffer("colAttr", GL_FLOAT, 2*sizeof(GL_FLOAT), 3, sizeof(Vertex));

    m_posunBuffer.bind();
    m_program->setAttributeArray("movementAttr", GL_FLOAT, 0, 1, 0 );


    m_functions->glDrawArrays(GL_TRIANGLES, 0, 9);

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
