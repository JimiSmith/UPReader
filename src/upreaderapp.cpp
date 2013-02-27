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

#include <QQuickView>
#include <QtQuick>

#include "upreader.h"
#include "feedmodel.h"
#include "filteredcontentmodel.h"
#include "article.h"
#include "upreaderapp.h"

static float getGridUnit()
{
    // Inspired by the UI toolkit’s code
    // (modules/Ubuntu/Components/plugin/ucunits.cpp)
    // as it is not publicly exposed.
    const char* envVar = "GRID_UNIT_PX";
    QByteArray stringValue = qgetenv(envVar);
    bool ok;
    float value = stringValue.toFloat(&ok);
    float defaultValue = 8;
    return ok ? value : defaultValue;
}

static float getQtWebkitDpr()
{
    const char* envVar = "QTWEBKIT_DPR";
    QByteArray stringValue = qgetenv(envVar);
    bool ok;
    float value = stringValue.toFloat(&ok);
    float defaultValue = 1.0;
    return ok ? value : defaultValue;
}


UPReaderApp::UPReaderApp(int& argc, char** argv) :
    QApplication(argc, argv),
    m_view(0)
{
}

void UPReaderApp::initialize()
{
    m_view = new QQuickView();
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->setTitle("UPReader");

    float gridUnit = getGridUnit();
    m_view->resize(40 * gridUnit, 68 * gridUnit);

    QObject::connect(m_view->engine(), SIGNAL(quit()), m_view, SLOT(close()));
    qmlRegisterType<Auth>("UPReader", 0, 1, "Auth");
    qmlRegisterType<FeedModel>("UPReader", 0, 1, "FeedModel");
    qmlRegisterType<FilteredContentModel>("UPReader", 0, 1, "ContentModel");
    qmlRegisterType<Subscription>("UPReader", 0, 1, "Subscription");
    qmlRegisterType<Article>("UPReader", 0, 1, "Article");
    qRegisterMetaType<QList<Subscription*> >("QList<Subscription*>");
    qRegisterMetaType<Subscription* >("Subscription*");
    m_view->engine()->addImportPath("qrc:/qml/");
    m_view->setSource(QUrl("qrc:/qml/main.qml"));
}

int UPReaderApp::exec()
{
    Q_ASSERT(m_view != 0);
    m_view->show();
    return QApplication::exec();
}
