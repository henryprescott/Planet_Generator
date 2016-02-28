#include "stdafx.h"
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include "ViewPort.h"
#include "NoiseGenerator.h"

int main(int argc, char *argv[])
{
	NoiseGenerator* noiseGen = new NoiseGenerator();
	noiseGen->init();

	QGuiApplication a(argc, argv);

	qmlRegisterType<ViewPort>("OpenGLUnderQML", 1, 0, "ViewPort");
	
	QQuickView viewer;
	viewer.setResizeMode(QQuickView::SizeRootObjectToView);
	viewer.setSource(QUrl("qrc:///QML/main.qml"));
	viewer.setTitle("Planet Generator");
	viewer.show(); 

	return a.exec();
}