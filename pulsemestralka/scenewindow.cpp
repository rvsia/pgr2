#include "scenewindow.h"

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

SceneWindow::SceneWindow()
    : m_program(nullptr),
      m_rotationAngle(0)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;
    delete m_cube;
}

void SceneWindow::initialize()
{
    /// Musime zapnout zpracovani hloubky
    m_functions->glEnable(GL_DEPTH_TEST);

    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_program->link();

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray(m_posAttr);
    m_program->enableAttributeArray(m_colAttr);

    /// Vytvoreni kostky ktera predstavuje scenu
    m_cube = new Cube(1, m_program, m_functions, "posAttr", "colAttr");
    m_land = new Land(0.3, m_program, m_functions, "posAttr", "colAttr");

    for( int a = 0; a < 6; a++ ) {
        m_floats[a] = RandomFloat(-0.1,1);
    }
}

void SceneWindow::render()
{
    /// Nabindujeme program se shadery
    /// Je dobre to udelat pred vykreslenim, protoze jich muze byt vice
    ///
    m_program->bind();

    /// Nastavime cast okna pres kterou budeme vykreslovat
    const qreal retinaScale = devicePixelRatio();
    m_functions->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    /// Vytvorime projekcni matici
    QMatrix4x4 projectionMatrix;
    /// Udelelame z ni jednotkovou matici
    projectionMatrix.setToIdentity();
    /// Nastavime, aby mela obsah takovy, aby reprezentovalo popsanou projekci
    projectionMatrix.perspective(30.0f, 4.0f/3.0f, 0.1f, 100.0f);

    /// Vytvorime pohledovou matici
    QMatrix4x4 modelViewMatrix;
    /// Pro zacatek bude opet jednotkova
    modelViewMatrix.setToIdentity();
    /// Posuneme pocatek souradneho systemu do uvedeneho mista
    modelViewMatrix.translate(-7,-2,-60.0);
    /// Otocime souradny system
    modelViewMatrix.rotate(0, 0.0f, 1.0f, 0.0f);

    QMatrix4x4 modelViewMatrixWithRotation = modelViewMatrix;

    /// Odesleme vynasobenou projekcni a pohledovou matici pro prvni objekt
    m_program->setUniformValue(m_matrixUniform, projectionMatrix * modelViewMatrix);
    /// Vymazeme scenu
    m_functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// Vykreslime kostku
    ///
    for( int a = 0; a < 6; a++ ) {
        modelViewMatrix.translate(m_floats[a]*20, 0, m_floats[a]*15);
        modelViewMatrix.rotate(m_floats[a]*100, 0.0f, 1.0f, 0.0f);
        m_program->setUniformValue(m_matrixUniform, projectionMatrix * modelViewMatrix);
        m_land->drawCube();
        modelViewMatrix.translate(-m_floats[a]*20, 0, -m_floats[a]*15);
        modelViewMatrix.rotate(-m_floats[a]*50, 0.0f, 1.0f, 0.0f);
    }


    modelViewMatrixWithRotation.translate(0,7,-1.0);
    modelViewMatrixWithRotation.rotate(m_rotationAngle, 0, -1, 0);

    m_program->setUniformValue(m_matrixUniform, projectionMatrix * modelViewMatrixWithRotation);
    m_cube->drawCube();

    /// Uvolnime program
    m_program->release();

    /// Testujeme, zda nenastala nejaka chyba
    GLenum err;
    while ((err = m_functions->glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err << "\n";
    }

    m_rotationAngle++;
    qDebug() << m_rotationAngle << ", ";
}
