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
#include "sqlhelper.h"

Manager::Manager(QObject* parent)
    : QObject(parent)
{
    m_netMan = new QNetworkAccessManager(this);
    connect(&m_watcher, SIGNAL(finished()), this, SIGNAL(updateSubList()));
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
    disconnect(&m_watcher, SIGNAL(finished()), this, SLOT(syncSubscriptions()));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(refreshSubscriptions()));
    QFuture<bool> subAddFuture = QtConcurrent::run(SqlHelper::setAllSubsToLoadingState);
    m_watcher.setFuture(subAddFuture);
}

void Manager::refreshSubscriptions()
{
    disconnect(&m_watcher, SIGNAL(finished()), this, SLOT(refreshSubscriptions()));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(syncSubscriptions()));
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
    QJsonDocument sd = QJsonDocument::fromJson(json);
    QVariant result = sd.toVariant();
    switch(m_operations.value(reply)) {
    case listOP: {
        QVariantList subList = result.toMap().value("subscriptions").toList();
        QFuture<bool> subAddFuture = QtConcurrent::run(SqlHelper::addOrUpdateSubBatch, subList);
        m_watcher.setFuture(subAddFuture);
    }
    }
}

void Manager::syncSubscriptions()
{
    QString google_id = SqlHelper::firstSubToUpdate();
    Subscription *s = new Subscription(m_accessToken, google_id, this);
    connect(s, SIGNAL(updated(Subscription*)), this, SLOT(subUpdated(Subscription*)));
    s->refresh();
}

void Manager::subUpdated(Subscription *sub)
{
    emit updateSub(sub->getId());
    sub->deleteLater();
    syncSubscriptions();
}

#include "manager.moc"
