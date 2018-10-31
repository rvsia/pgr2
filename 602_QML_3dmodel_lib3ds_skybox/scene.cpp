/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include "scene.h"


Scene::Scene()
    : m_program(nullptr)
    , m_rotationAngle(0)
    , m_thread_rotationAngle(0)
    , m_model(nullptr)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)),
            this, SLOT(handleWindowChanged(QQuickWindow*)));
}

void Scene::setRotationAngle(qreal rotationAngle)
{
    if (rotationAngle == m_rotationAngle)
        return;
    m_rotationAngle = rotationAngle;
    emit rotationAngleChanged();
    if (window())
        window()->update();
}

void Scene::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection
        connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);

        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void Scene::initTextures()
{
    /*
    m_texture = new QOpenGLTexture(QImage(":/cube.png"));
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    /// Mam jediny texturovaci objekt, takze to muzu nabindovat hned ted
    m_texture->bind();
    */
}

void Scene::init(){
    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_skyboxProgram = new QOpenGLShaderProgram();
    m_skyboxProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.vsh");
    m_skyboxProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.fsh");
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_skyboxProgram->link();

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_skybox_posAttr = m_skyboxProgram->attributeLocation("a_position");
    m_skybox_texAttr = m_skyboxProgram->attributeLocation("a_texcoord");
    m_skybox_primaryTexture = m_skyboxProgram->uniformLocation("primaryTexture");
    m_skybox_matrixUniform = m_skyboxProgram->uniformLocation("matrix");

    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basic.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.frag");
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_program->link();

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_posAttr = m_program->attributeLocation("a_Vertex");
    m_normAttr = m_program->attributeLocation("a_Normal");
    m_projectionMatrixUniform = m_program->uniformLocation("projection_matrix");
    m_modelviewMatrixUniform = m_program->uniformLocation("modelview_matrix");

    /// Nesmime zapomenout atributy "zapnout", jinak se nic nevykresli
    m_program->enableAttributeArray(m_posAttr);
    m_program->enableAttributeArray(m_normAttr);

    try{
        /// Vytvoreni objektu ktery predstavuje scenu
        //std::string url = "/Users/davidprochazka/Dropbox/_my_courses/PG2/OpenGL\ QtProjekty/503_komplexni_scena_3D_objekt_ze_souboru_lib3ds/models/monkey/monkey.3ds";
        std::string url = "‎⁨/Users/xvsiansk/Desktop/git/602_QML_3dmodel_lib3ds_skybox/models/monkey/monkey.3ds";
        m_model = new CModel3DS(url, m_program, "a_Vertex", "a_Normal");
    } catch(std::string error_str){
        qDebug() << "Error: " << error_str.c_str() << "\n";
        exit(1);
    }

    m_model->CreateVBO();

    m_skybox = new Skybox(2, m_skyboxProgram, "a_position", "primaryTexture",
                          {":/textures/sp3right.jpg",
                           ":/textures/sp3left.jpg",
                           ":/textures/sp3top.jpg",
                           ":/textures/sp3bot.jpg",
                           ":/textures/sp3front.jpg",
                           ":/textures/sp3back.jpg"
                          });

    connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()),
            this, SLOT(cleanup()), Qt::DirectConnection);
}

/// Musime zapnout zpracovani hloubky. Toto musim bohuzel delat pri kazdem vykresleni
/// Nestaci pouze v initu, to stejne s mazaci barvou atp.
void Scene::cleanScene(){
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glClearDepth(1);
    glClearColor(0.8,0.8,0.8,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::setLight(){
    m_program->setUniformValue("material0.ambient", 0.5f, 0.0f, 0.0f, 1.0f);
    m_program->setUniformValue("material0.diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
    m_program->setUniformValue("material0.specular", 0.5f, 0.5f, 0.5f, 1.0f);
    m_program->setUniformValue("material0.emissive", 0.0f, 0.0f, 0.0f, 1.0f);
    m_program->setUniformValue("material0.shininess", 5.0f);

    m_program->setUniformValue("light0.ambient", 0.4f, 0.4f, 0.4f, 1.0f);
    m_program->setUniformValue("light0.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    m_program->setUniformValue("light0.specular", 0.3f, 0.3f, 0.3f, 1.0f);

    m_program->setUniformValue("light0.position", 0.0f, 0.0f, 10.0f, 0.0f);
    m_program->setUniformValue("light0.spot_direction", 0.0f, 0.0f, -5.0f);
    m_program->setUniformValue("light0.spot_cut_off", 0.85f);
    m_program->setUniformValue("light0.spot_exponent", 2.0f);

    m_program->setUniformValue("light0.constant_attenuation", 0.0f);
    m_program->setUniformValue("light0.linear_attenuation", 0.08f);
    m_program->setUniformValue("light0.quadratic_attenuation", 0.001f);
}

void Scene::paint()
{
    if (!m_program) {
        init();
        qDebug() << "Initialized" << "\n";
    }

    cleanScene();

    /// Nastavime cast okna pres kterou budeme vykreslovat
    const qreal retinaScale = 1;// devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    /// Vytvorime projekcni matici
    QMatrix4x4 projectionMatrix;
    /// Udelelame z ni jednotkovou matici
    projectionMatrix.setToIdentity();
    /// Nastavime, aby mela obsah takovy, aby reprezentovalo popsanou projekci
    projectionMatrix.perspective(60.0f, 4.0f/3.0f, 1.0f, 10000.0f);
    //projectionMatrix.ortho(-5.0, 5.0, -5.0, 5.0, 0.0, 100.0);

    /// Vytvorime pohledovou matici
    QMatrix4x4 modelViewMatrix;
    /// posuneme jeste jednou souradny system
    modelViewMatrix.setToIdentity();
    modelViewMatrix.lookAt(
                        {0.0,0.0,0.0},
                        {0.0,0.0, -1},
                        {0.0,1.0,0.0});

    m_skyboxProgram->bind();
    // je dobre zrusit translaci kamery
    m_skyboxProgram->setUniformValue(m_skybox_matrixUniform, projectionMatrix * modelViewMatrix);
    m_skybox->draw();
    m_skyboxProgram->release();

    modelViewMatrix.translate(0,0,-4);
    modelViewMatrix.rotate(-90, 1, 0, 0);
    modelViewMatrix.rotate(m_thread_rotationAngle, 0,0,1);

    /// Nabindujeme program se shadery
    /// Je dobre to udelat pred vykreslenim, protoze jich muze byt vice
    m_program->bind();
    /// Odeslu nastaveni svetel
    setLight();
    /// odesleme upravenou projekcni a pohledovou matici
    m_program->setUniformValue(m_modelviewMatrixUniform, modelViewMatrix);
    m_program->setUniformValue(m_projectionMatrixUniform, projectionMatrix);
    /// a znovu vykreslime objekt
    m_model->Draw();
    /// Uvolnime program
    m_program->release();

    /// Testujeme, zda nenastala nejaka chyba
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err << "\n";
    }
}

void Scene::cleanup()
{
    /// Nejsem si jisty, jestli je to optimalni uvolneni
    /// ale delete volat nelze, mame vice vlaken
}

void Scene::sync()
{
    m_thread_rotationAngle = m_rotationAngle;
}

