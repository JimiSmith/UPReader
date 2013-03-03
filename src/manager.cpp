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

#include <QtCore/QDebug>
#include <QJsonDocument>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "manager.h"

Manager::Manager(QObject* parent)
    : QObject(parent)
{
    m_netMan = new QNetworkAccessManager(this);
    connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinshed(QNetworkReply*)));
}

Manager::~Manager()
{

}

void Manager::setAccessToken(QString access)
{
    m_accessToken = access;
}

void Manager::setRefreshToken(QString refresh)
{
    m_refreshToken = refresh;
}

void Manager::refreshSubList()
{
    // cancel all running operations
    foreach (QNetworkReply* reply, m_operations.keys()) {
        reply->abort();
    }
    QNetworkRequest r(QUrl("https://www.google.com/reader/api/0/subscription/list?output=json"));
    r.setRawHeader("Authorization", QString("OAuth %0").arg(m_accessToken).toUtf8());
    m_operations.insert(m_netMan->get(r), listOP);
}

void Manager::replyFinshed(QNetworkReply* reply)
{
    QByteArray json = reply->readAll();
//    qDebug() << json;
    QJsonDocument sd = QJsonDocument::fromJson(json);
    QVariant result = sd.toVariant();
    switch(m_operations.value(reply)) {
    case listOP: {
        QVariantList t = result.toMap().value("subscriptions").toList();
        foreach(QVariant v, t) {
            addOrUpdateSub(v.toMap());
        }
        emit updateSubList();
    }
    }
    syncSubscriptions();
}

bool Manager::addOrUpdateSub(QVariantMap subData)
{
    QSqlDatabase::database().transaction();

    QString id = subData.value("id").toString();

    id.replace("?", "%3F");
    id.replace("=", "%3D");

    QSqlQuery existsQuery;
    existsQuery.prepare("SELECT count(google_id) FROM subscriptions WHERE google_id=:google_id");
    existsQuery.bindValue(":google_id", id);
    bool existsRet = existsQuery.exec();

    if (!existsRet) {
        QSqlDatabase::database().rollback();
        return false;
    }

    bool exists = existsQuery.next() && existsQuery.value(0).toInt() > 0;

    QSqlQuery dataQuery;

    if (exists) {
        dataQuery.prepare("UPDATE subscriptions SET title=:title, needs_update=1, url=:url, continuation=NULL WHERE google_id=:google_id");
    } else {
        dataQuery.prepare("INSERT INTO subscriptions (google_id, title, url, unread, continuation, needs_update) VALUES (:google_id, :title, :url, 0, NULL, 1)");
    }

    dataQuery.bindValue(":google_id", id);
    dataQuery.bindValue(":title", subData.value("title").toString());
    dataQuery.bindValue(":url", subData.value("htmlUrl").toString());

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();

    return true;
}

bool Manager::syncSubscriptions()
{
    QSqlQuery query;
    query.exec("SELECT google_id FROM subscriptions WHERE needs_update=1 LIMIT 1");
    if (query.next()) {
        QString google_id = query.value(0).toString();
        Subscription *s = new Subscription(m_accessToken, google_id, this);
        connect(s, SIGNAL(updated(Subscription*)), this, SLOT(subUpdated(Subscription*)));
        s->refresh();

    }
}

void Manager::subUpdated(Subscription *sub)
{
    emit updateSub(sub->getId());
    sub->deleteLater();
    syncSubscriptions();
}

#include "manager.moc"
