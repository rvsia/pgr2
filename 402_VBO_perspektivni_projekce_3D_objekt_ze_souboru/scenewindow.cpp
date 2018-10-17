#include "scenewindow.h"
#include <QMouseEvent>

SceneWindow::SceneWindow()
    : m_program(nullptr),
      m_rotationAngle(0)
{
}

SceneWindow::~SceneWindow(){
    delete m_program;
    delete m_model;
    /// Uvolnim texturu z pameti
    m_texture->destroy();
}

void SceneWindow::initialize()
{
    /// Musime zapnout zpracovani hloubky
    m_functions->glEnable(GL_DEPTH_TEST);

    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh");
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_program->link();

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_posAttr = m_program->attributeLocation("a_position");
    m_texAttr = m_program->attributeLocation("a_texcoord");
    m_matrixUniform = m_program->uniformLocation("matrix");

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray(m_posAttr);
    m_program->enableAttributeArray(m_texAttr);

    /// Vytvoreni lode ktera predstavuje scenu
    m_model = new Model3d("/Users/xvsiansk/Desktop/_cviceni_01 samostatna_pole_bez_shaderu_kostra/402_VBO_perspektivni_projekce_3D_objekt_ze_souboru/models/spaceship/spaceship.3ds",
                          m_program, m_functions, "a_position", "a_texcoord");

    /// Inicializuj texturovaci objekt
    initTextures();

    /// Spust casovac
    m_timer.start(12, this);
}


void SceneWindow::initTextures()
{
    m_texture = new QOpenGLTexture(QImage(":/models/spaceship/spaceshiptexture.bmp"));

    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    /// Mam jediny texturovaci objekt, takze to muzu nabindovat hned ted
    m_texture->bind();
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
    projectionMatrix.perspective(35.0f, 4.0f/3.0f, 10.0f, 10000.0f);

    /// Vytvorime pohledovou matici
    QMatrix4x4 modelViewMatrix;

    /// Vymazeme scenu
    m_functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// posuneme jeste jednou souradny system
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(-1,-1,-300.0);
    modelViewMatrix.rotate(m_rotation);
    /// odesleme upravenou pohledovou matici
    m_program->setUniformValue(m_matrixUniform, projectionMatrix * modelViewMatrix);

    /// a znovu vykreslime kostku
    m_model->draw();

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

// Nasledujici kod je ze vzoroveho prikladu na Qt a OpenGL ES

void SceneWindow::mousePressEvent(QMouseEvent *e)
{
    /// Ulozim si pozici, ktera reprezentuje prvni bod kliknuti
    m_mousePressPosition = QVector2D(e->localPos());
}

void SceneWindow::mouseReleaseEvent(QMouseEvent *e)
{
    /// Spocitam rozdil mezi mistem dotyku a mistem stisku
    QVector2D diff = QVector2D(e->localPos()) - m_mousePressPosition;

    /// Nachsytam si rotacni vektor
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    /// a nastavim si rychlost rotace. Ta je odvozena z delky pohybu
    qreal acc = diff.length() / 100.0;

    /// Spocitam novou rotacni osu
    m_rotationAxis = (m_rotationAxis * m_angularSpeed + n * acc).normalized();

    /// Zvysim rotacni rychlost
    m_angularSpeed += acc;
}

void SceneWindow::timerEvent(QTimerEvent *)
{
    /// Snizim rotacni rychlost
    m_angularSpeed *= 0.99;

    /// Zastavim rotaci, pokud se rychlost rotace dostala pod prah detekce
    if (m_angularSpeed < 0.01) {
        m_angularSpeed = 0.0;
    } else {
        /// Prepocitam rotaci (uhel otoceni)
        m_rotation = QQuaternion::fromAxisAndAngle(m_rotationAxis, m_angularSpeed) * m_rotation;

        /// Zavolam update sceny
        renderNow();
    }
}
