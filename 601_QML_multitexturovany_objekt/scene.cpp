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

void Scene::init(){
    /// Vytvorime program pro shader a posleme mu zdrojove kody shaderu
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.fsh");
    /// Linkuji shader, to by melo byt udelano pred zaregistrovanim atributu a un. prom.
    m_program->link();

    /// Zaregistrujeme atributy a uniformni hodnoty, ktere se budou shaderum posilat
    m_posAttr = m_program->attributeLocation("a_position");
    m_texAttr = m_program->attributeLocation("a_texcoord");
    m_secondTexAttr = m_program->attributeLocation("a_secondTexcoord");

    m_primaryTexture = m_program->uniformLocation("primaryTexture");
    m_secondaryTexture = m_program->uniformLocation("secondaryTexture");
    m_matrixUniform = m_program->uniformLocation("matrix");

    /// Vytvoreni kostky ktera predstavuje scenu
    m_cube = new Cube(1, m_program, "a_position", "a_texcoord", "a_secondTexcoord", "primaryTexture", "secondaryTexture");

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
    glClearColor(0.8,0.8,0.8,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::paint()
{
    if (!m_program) {
        init();
        qDebug() << "Initialized" << "\n";
    }

    cleanScene();

    /// Nabindujeme program se shadery
    /// Je dobre to udelat pred vykreslenim, protoze jich muze byt vice
    /// a take textury
    m_program->bind();

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
    modelViewMatrix.translate(-0.5,-0.5,-13.0);
    /// Otocime souradny system
    modelViewMatrix.rotate(m_thread_rotationAngle, 0.0f, 1.0f, 0.0f);

    /// Odesleme vynasobenou projekcni a pohledovou matici pro prvni objekt
    m_program->setUniformValue(m_matrixUniform, projectionMatrix * modelViewMatrix);

    /// Nastavime cast okna pres kterou budeme vykreslovat
    const qreal retinaScale = 1;
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    /// Vykreslime kostku
    m_cube->draw();

    /// posuneme jeste jednou souradny system
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(-1,-1,-10.0);
    modelViewMatrix.rotate(45, 1.0f, 1.0f, 0.0f);
    /// odesleme upravenou pohledovou matici
    m_program->setUniformValue(m_matrixUniform, projectionMatrix * modelViewMatrix);

    /// a znovu vykreslime kostku
    m_cube->draw();

    /// Uvolnime program
    m_program->release();

    /// Testujeme, zda nenastala nejaka chyba
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error: " << err << "\n";
    }
    //qDebug() << m_thread_rotationAngle << ", ";
}

void Scene::cleanup()
{
    /// ToDo: dopsat uvolneni objektu (pozor mame vice vlaken)
}

void Scene::sync()
{
    m_thread_rotationAngle = m_rotationAngle;
}

