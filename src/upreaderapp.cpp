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
#include "filteredfeedmodel.h"
#include "contentmodel.h"
#include "article.h"
#include "upreaderapp.h"
#include "sqlhelper.h"

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
    //open sqlite DB
    setupDB();

    m_view = new QQuickView();
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->setTitle("UPReader");

    float gridUnit = getGridUnit();
    m_view->resize(40 * gridUnit, 68 * gridUnit);

    QObject::connect(m_view->engine(), SIGNAL(quit()), m_view, SLOT(close()));
    qmlRegisterType<Auth>("UPReader", 0, 1, "Auth");
    qmlRegisterType<FeedModel>("UPReader", 0, 1, "FeedModel");
    qmlRegisterType<FilteredFeedModel>("UPReader", 0, 1, "FilteredFeedModel");
    qmlRegisterType<ContentModel>("UPReader", 0, 1, "ContentModel");
    qmlRegisterType<Subscription>("UPReader", 0, 1, "Subscription");
    qmlRegisterType<Article>("UPReader", 0, 1, "Article");
    qRegisterMetaType<QList<Subscription*> >("QList<Subscription*>");
    qRegisterMetaType<Subscription* >("Subscription*");
    qRegisterMetaType<QAbstractItemModel* >("QAbstractItemModel*");
    m_view->engine()->addImportPath("qrc:/qml/");
    m_view->setSource(QUrl("qrc:/qml/main.qml"));

    connect(this, SIGNAL(aboutToQuit()), this, SLOT(onExit()));
}

int UPReaderApp::exec()
{
    Q_ASSERT(m_view != 0);
    m_view->show();
    return QApplication::exec();
}

void UPReaderApp::onExit()
{
    qDebug() << "Going down";
    m_appDB.close();
}

void UPReaderApp::setupDB()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir = QDir(path);
    dir.mkpath(path);
    QString absPath = dir.absoluteFilePath("up-reader.db");
    m_appDB = QSqlDatabase::addDatabase("QSQLITE");
    m_appDB.setDatabaseName(absPath);
    m_appDB.open();

    if (m_appDB.isOpen()) {
        QStringList tableValues;
        tableValues.append("id integer primary key");
        tableValues.append("title text not null");
        tableValues.append("url text");
        tableValues.append("continuation text");
        tableValues.append("google_id text not null");
        tableValues.append("unread integer");
        tableValues.append("needs_update integer");
        SqlHelper::createTableIfNeeded(QString("subscriptions"), tableValues);

        QStringList articleValues;
        articleValues.append("id integer primary key");
        articleValues.append("subscription_id integer not null");
        articleValues.append("content text not null");
        articleValues.append("summary text");
        articleValues.append("title text not null");
        articleValues.append("link text");
        articleValues.append("published text");
        articleValues.append("updated text");
        articleValues.append("author text");
        articleValues.append("read integer");
        articleValues.append("article_domain_name text");
        articleValues.append("google_id text not null");
        SqlHelper::createTableIfNeeded(QString("articles"), articleValues);

        QStringList stateJoinValues;
        stateJoinValues.append("article_id integer not null");
        stateJoinValues.append("state_id integer not null");
        SqlHelper::createTableIfNeeded(QString("article_states"), stateJoinValues);

        QStringList stateValues;
        stateValues.append("id integer primary key");
        stateValues.append("name text not null");
        SqlHelper::createTableIfNeeded(QString("states"), stateValues);


    } else {
        qWarning() << "DB not open";
    }
}
