#include "scenewindow.h"

SceneWindow::SceneWindow()
    : m_program(nullptr),
      m_rotationAngle(0)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;

    for(auto object:m_sceneContent){
        delete object;
    }
}

void SceneWindow::initialize()
{
    /// Musime zapnout zpracovani hloubky
    m_functions->glEnable(GL_DEPTH_TEST);

    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/basic.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/basic.frag");

    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    if (!m_program->link()){
        qDebug() << "Can not link the shader program\n";
    }

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    const std::string vertexAtrName = "a_Vertex";
    const std::string normalAtrName = "a_Normal";

    m_positionAttr = m_program->attributeLocation(vertexAtrName.c_str());
    m_normalAttr = m_program->attributeLocation(normalAtrName.c_str());

    m_projectionMatrixUniform = m_program->uniformLocation("projection_matrix");
    m_modelViewMatrixUniform = m_program->uniformLocation("modelview_matrix");

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray(m_positionAttr);
    m_program->enableAttributeArray(m_normalAttr);

    /// Vytvoreni kostky ktera predstavuje jeden prvek sceny
    m_sceneContent.push_back(new Cube(1, m_program, m_functions, vertexAtrName, normalAtrName));
}

void SceneWindow::render()
{
    /// Nabindujeme program se shadery
    /// Je dobre to udelat pred vykreslenim, protoze jich muze byt vice
    m_program->bind();

    /// Nastavime cast okna pres kterou budeme vykreslovat
    const qreal retinaScale = devicePixelRatio();
    m_functions->glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    /// Vytvorime projekcni matici
    QMatrix4x4 projectionMatrix;
    /// Udelelame z ni jednotkovou matici
    projectionMatrix.setToIdentity();
    /// Nastavime, aby mela obsah takovy, aby reprezentovalo popsanou projekci
    projectionMatrix.perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);

    /// Vytvorime pohledovou matici
    QMatrix4x4 modelViewMatrix;
    /// Pro zacatek bude opet jednotkova
    modelViewMatrix.setToIdentity();
    /// Posuneme pocatek souradneho systemu do uvedeneho mista
    modelViewMatrix.translate(0.0,0.0,-7.0);
    /// Otocime souradny system
    modelViewMatrix.rotate(m_rotationAngle, 0.0f, 1.0f, 0.0f);

    /// Odesleme vynasobenou projekcni a pohledovou matici pro prvni objekt    
    m_program->setUniformValue("modelview_matrix", modelViewMatrix);
    m_program->setUniformValue("projection_matrix", projectionMatrix);

    m_program->setUniformValue("material0.ambient", 0.9f, 0.0f, 0.0f, 1.0f);
    m_program->setUniformValue("material0.diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
    m_program->setUniformValue("material0.specular", 0.1f, 0.1f, 0.1f, 1.0f);
    m_program->setUniformValue("material0.emissive", 0.0f, 0.0f, 0.0f, 1.0f);
    m_program->setUniformValue("material0.shininess", 2.0f);

    m_program->setUniformValue("light0.ambient", 0.6f, 0.6f, 0.6f, 1.0f);
    m_program->setUniformValue("light0.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    m_program->setUniformValue("light0.specular", 0.3f, 0.3f, 0.3f, 1.0f);
    m_program->setUniformValue("light0.position", -10.0f, 0.0f, 1.0f, 0.0f);

    /// Vymazeme scenu
    m_functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// Vykreslime kostku
    m_sceneContent.at(0)->draw();

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
