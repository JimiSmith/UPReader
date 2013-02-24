#include <QGuiApplication>
#include <QQuickView>
#include <QtQuick>

#include "upreader.h"
#include "feedmodel.h"
#include "filteredcontentmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView* viewer = new QQuickView();
    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
	QObject::connect(viewer->engine(), SIGNAL(quit()), viewer, SLOT(close()));
    qmlRegisterType<Auth>("UPReader", 0, 1, "Auth");
    qmlRegisterType<FeedModel>("UPReader", 0, 1, "FeedModel");
    qmlRegisterType<FilteredContentModel>("UPReader", 0, 1, "ContentModel");
    qmlRegisterType<Subscription>("UPReader", 0, 1, "Subscription");
	qRegisterMetaType<QList<Subscription*> >("QList<Subscription*>");
	qRegisterMetaType<Subscription* >("Subscription*");
    viewer->setSource(QUrl("../src/qml/main.qml"));
    viewer->show();

    return app.exec();
}
