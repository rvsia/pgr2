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

#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QBasicTimer>
#include "cube.h"

class Scene : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal rotationAngle READ rotationAngle WRITE setRotationAngle NOTIFY rotationAngleChanged)

public:
    Scene();

    qreal rotationAngle() const { return m_rotationAngle; }
    void setRotationAngle(qreal rotationAngle);

signals:
    void rotationAngleChanged();

public slots:
    void paint();
    void cleanup();
    void sync();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void init();
    void cleanScene();

    /// Identifikator tranformacni matice, ktera se posida do shaderu
    GLuint m_matrixUniform;
    /// Identifikator atributu pozice vrcholu, ktera se posida do shaderu
    GLuint m_posAttr;
    /// Identifikator atributu textury vrcholu, ktera se posida do shaderu
    GLuint m_texAttr;
    /// Identifikator atributu sekundarni textury vrcholu, ktera se posida do shaderu
    GLuint m_secondTexAttr;

    /// Identifikator prvni textury
    GLuint m_primaryTexture;
    /// Identifikator sekundarni textury
    GLuint m_secondaryTexture;

    /// Objekt, ktery je soucasti sceny a bude vykreslovan
    Cube* m_cube;
    /// Shader, kteremu budeme posilat data
    QOpenGLShaderProgram *m_program;

    // -----------------------------------------------------------------
    //                         Atributy pro otoceni z QML
    // -----------------------------------------------------------------
    /// Pomocna promenna s uhlem otoceni pro automaticky se otacejici kostku
    qreal m_rotationAngle;
    /// Promenna ktera se pouziva pro synchronizaci animace
    qreal m_thread_rotationAngle;
};

#endif // SQUIRCLE_H
