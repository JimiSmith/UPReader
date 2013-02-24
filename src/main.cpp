/*
    A Google Reader client for Ubuntu Phone
    Copyright (C) 2013  Jimi Smith <smithj002@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <QGuiApplication>
#include <QQuickView>
#include <QtQuick>

#include "upreader.h"
#include "feedmodel.h"
#include "filteredcontentmodel.h"
#include "article.h"

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
    qmlRegisterType<Article>("UPReader", 0, 1, "Article");
    qRegisterMetaType<QList<Subscription*> >("QList<Subscription*>");
    qRegisterMetaType<Subscription* >("Subscription*");
    viewer->setSource(QUrl("../src/qml/main.qml"));
    viewer->show();

    return app.exec();
}
