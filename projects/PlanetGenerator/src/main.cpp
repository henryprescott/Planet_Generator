#include "stdafx.h"
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include "ViewPort.h"

int main(int argc, char *argv[])
{
	QGuiApplication a(argc, argv);

	qmlRegisterType<ViewPort>("OpenGLUnderQML", 1, 0, "ViewPort");
	
	QQuickView viewer;
	viewer.setResizeMode(QQuickView::SizeRootObjectToView);
	viewer.setSource(QUrl("qrc:///QML/main.qml"));
	viewer.setTitle("Planet Generator");
	viewer.show(); 

	return a.exec();
}
