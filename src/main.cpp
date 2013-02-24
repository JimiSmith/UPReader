#include <QGuiApplication>
#include <QQuickView>
#include <QtQuick>

#include "qgreader.h"
#include "feedmodel.h"
#include "filteredcontentmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView* viewer = new QQuickView();
    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
	QObject::connect(viewer->engine(), SIGNAL(quit()), viewer, SLOT(close()));
	qmlRegisterType<Auth>("QGReader", 0, 1, "Auth");
	qmlRegisterType<FeedModel>("QGReader", 0, 1, "FeedModel");
	qmlRegisterType<FilteredContentModel>("QGReader", 0, 1, "ContentModel");
    qmlRegisterType<Subscription>("QGReader", 0, 1, "Subscription");
	qRegisterMetaType<QList<Subscription*> >("QList<Subscription*>");
	qRegisterMetaType<Subscription* >("Subscription*");
    viewer->setSource(QUrl("../src/qml/main.qml"));
    viewer->show();

    return app.exec();
}
