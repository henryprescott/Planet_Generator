#pragma once
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

class ViewPortRenderer : public QObject, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	ViewPortRenderer();
	~ViewPortRenderer();

	void setT(qreal t);
	void setViewportSize(const QSize &size);
	void setWindow(QQuickWindow *window);

public slots:
	void paint();
	void update();

private:
	QSize m_viewportSize;
	qreal m_t;
	QOpenGLShaderProgram *m_program;
	QQuickWindow *m_window;
};

class ViewPort : public QQuickItem
{
	Q_OBJECT
		Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
	ViewPort();

	qreal t() const;
	void setT(qreal t);

signals:
	void tChanged();

	public slots:
	void sync();
	void cleanup();

	private slots:
	void handleWindowChanged(QQuickWindow *win);

private:
	qreal m_t;
	ViewPortRenderer *m_renderer;
};
