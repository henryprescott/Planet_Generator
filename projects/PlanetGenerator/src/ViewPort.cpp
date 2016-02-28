/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "stdafx.h"
#include "ViewPort.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

ViewPortRenderer::ViewPortRenderer() : 
	m_t(0),
	m_program(0)
{
}

ViewPortRenderer::~ViewPortRenderer()
{
	delete m_program;
}

void ViewPortRenderer::paint()
{
	if (!m_program)
	{
		initializeOpenGLFunctions();

		m_program = new QOpenGLShaderProgram();
		m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
			"attribute highp vec4 vertices;"
			"varying highp vec2 coords;"
			"void main() {"
			"    gl_Position = vertices;"
			"    coords = vertices.xy;"
			"}");
		m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
			"uniform lowp float t;"
			"varying highp vec2 coords;"
			"void main() {"
			"    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
			"    i = smoothstep(t - 0.8, t + 0.8, i);"
			"    i = floor(i * 20.) / 20.;"
			"    gl_FragColor = vec4(coords * .5 + .5, i, i);"
			"}");

		m_program->bindAttributeLocation("vertices", 0);
		m_program->link();

	}

	m_program->bind();

	m_program->enableAttributeArray(0);

	float values[] =
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};

	m_program->setAttributeArray(0, GL_FLOAT, values, 2);
	m_program->setUniformValue("t", (float)m_t);

	glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

	glDisable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	m_program->disableAttributeArray(0);
	m_program->release();

	// Not strictly needed for this example, but generally useful for when
	// mixing with raw OpenGL.
	m_window->resetOpenGLState();
}

void ViewPortRenderer::setT(qreal t)
{
	m_t = t;
}

void ViewPortRenderer::setViewportSize(const QSize &size)
{
	m_viewportSize = size;
}

void ViewPortRenderer::setWindow(QQuickWindow *window)
{
	m_window = window;
}

void ViewPortRenderer::update()
{
	int i = 0;
}

/////////////////////////////////////////////

ViewPort::ViewPort() : 
	m_t(0),
	m_renderer(0)
{
	connect(this, &QQuickItem::windowChanged, this, &ViewPort::handleWindowChanged);
}

qreal ViewPort::t() const 
{ 
	return m_t; 
}

void ViewPort::setT(qreal t)
{
	if (t == m_t)
	{
		return;
	}

	m_t = t;
	emit tChanged();

	if (window())
	{
		window()->update();
	}
}

void ViewPort::handleWindowChanged(QQuickWindow *win)
{
	if (win) 
	{
		connect(win, &QQuickWindow::beforeSynchronizing, this, &ViewPort::sync, Qt::DirectConnection);
		connect(win, &QQuickWindow::sceneGraphInvalidated, this, &ViewPort::cleanup, Qt::DirectConnection);
		// If we allow QML to do the clearing, they would clear what we paint
		// and nothing would show.
		win->setClearBeforeRendering(false);
	}
}

void ViewPort::cleanup()
{
	if (m_renderer)
	{
		delete m_renderer;
		m_renderer = 0;
	}
}

void ViewPort::sync()
{
	if (!m_renderer)
	{
		m_renderer = new ViewPortRenderer();
		connect(window(), &QQuickWindow::beforeRendering, m_renderer, &ViewPortRenderer::paint, Qt::DirectConnection);
		connect(window(), &QQuickWindow::frameSwapped, m_renderer, &ViewPortRenderer::update, Qt::DirectConnection);
	}

	m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
	m_renderer->setT(m_t);
	m_renderer->setWindow(window());
}
