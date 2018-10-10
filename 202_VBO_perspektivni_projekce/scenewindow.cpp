#include "scenewindow.h"

SceneWindow::SceneWindow()
    : m_program(nullptr)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;
    m_ctverecBuffer.destroy();
    m_trojuhelnikBuffer.destroy();
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
    /// pridame uniformni promennou
    m_matrixUniform = m_program->uniformLocation("matrix");

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");


    /// Vytvorime si pomocne pole s vrcholy
    Vertex ctverec[] = {
        { {1.0, 0.0, -1.9}, {1.0, 0.0, 0.0} },
        { {0.0, 0.0, -1.9}, {0.0, 1.0, 0.0} },
        { {1.0, 1.0, -1.9}, {0.0, 0.0, 1.0} },
        { {0.0, 1.0, -1.9}, {1.0, 0.0, 1.0} }
    };

    /// Vytvorime buffer a ulozime do nej pole
    m_ctverecBuffer.create();
    m_ctverecBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_ctverecBuffer.bind();
    m_ctverecBuffer.allocate( ctverec, sizeof(ctverec) );

    /// vytvorime jine pole hodnot pro druhy objekt
    Vertex troujhelnik[] = {
        { { 0.0,-1.0, -15}, {1.0, 1.0, 0.0}},
        { {-1.0,-1.0, -15}, {0.0, 1.0, 1.0}},
        { {-1.0, 0.0, -15}, {1.0, 0.0, 1.0}}
    };

    /// a opet pro nej vytvorime buffer a naplnime jej daty
    m_trojuhelnikBuffer.create();
    m_trojuhelnikBuffer.setUsagePattern( QOpenGLBuffer::StreamDraw );
    m_trojuhelnikBuffer.bind();
    m_trojuhelnikBuffer.allocate( troujhelnik, sizeof(troujhelnik) );
}

void SceneWindow::render()
{
    /// Nabindujeme program se shadery
    m_program->bind();

    /// Nastavime vykreslovani na obrazovku
    const qreal retinaScale = devicePixelRatio();
    m_functions->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    /// Vytvorime projekcni matici
    QMatrix4x4 projectionMatrix;
    /// Nejdrive ji naplnime jednotkovou matici
    projectionMatrix.setToIdentity();
    /// Nasledni ji nastavime tak, aby odpovidala definovane projekci
    projectionMatrix.perspective(10.0f, 4.0f/3.0f, 0.0f, 100.0f);

    m_program->setUniformValue(m_matrixUniform, projectionMatrix);

    /// Vymazeme plochu pro vykreslovani
    //m_functions->glClear(GL_COLOR_BUFFER_BIT);

    /// Nabindujeme buffer, nastavime ukazatel a vykreslime
    m_ctverecBuffer.bind();

    /// Aktivujeme hodnoty, ktere se budou do shaderu posilat
    m_program->enableAttributeArray("posAtr");
    m_program->enableAttributeArray("colAttr");

    /// Nastavime ukazatele pro cteni dat
                                    //name, type, offset, tupleSize, stride
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));
    m_program->setAttributeBuffer("colAttr", GL_FLOAT, 3*sizeof(GL_FLOAT), 3, sizeof(Vertex));

    /// a provedeme vykresleni jednoho objektu
    m_functions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /// a vykreslime druhy objekt
    m_trojuhelnikBuffer.bind();
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));
    m_program->setAttributeBuffer("colAttr", GL_FLOAT, 3*sizeof(GL_FLOAT), 3, sizeof(Vertex));
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
