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
    m_subList = QList< Subscription* >();
// 	qDebug() << "refreshing" << m_accessToken;
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
            QVariantMap m = v.toMap();
            Subscription* s = new Subscription(m_accessToken, this);
            s->setId(m.value("id").toString());
            s->setTitle(m.value("title").toString());
            s->setSortId(m.value("sortid").toString());
            s->setOldestItemTime(m.value("firstitemmsec").toInt());
            s->setUrl(m.value("htmlUrl").toString());
            s->setCategories(m.value("categories").toStringList());
            m_subList.append(s);
        }
        emit updatedSubList(m_subList);
    }
    }
}

#include "manager.moc"
