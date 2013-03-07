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
#include <QtCore/qdatetime.h>
#include <QThread>

#include "subscription.h"
#include "article.h"
#include "apihelper.h"
#include "sqlhelper.h"
#include "networkmanager.h"

Subscription::Subscription()
{
    Subscription("", "");
}

Subscription::Subscription(QString token, QString id, QObject* parent)
    : QObject(parent)
{
    m_accessToken = token;
    m_id = id;
    m_netMan = new NetworkManager(this);
    m_parser = new FeedParser();
    connect(m_parser, SIGNAL(doneParsing()), this, SLOT(parsingComplete()));
}

Subscription::~Subscription()
{
    delete m_parser;
}

QString Subscription::getTitle()
{
    return "Test";
}

QString Subscription::getId()
{
    return m_id;
}

QString Subscription::getContinuationToken()
{
}

int Subscription::getDBId()
{
    return SqlHelper::subIdForGoogleId(m_id);
}

void Subscription::handleNetworkReply(QNetworkReply *reply)
{
    m_atomText = QString::fromUtf8(reply->readAll());
    m_parser->setFeedString(m_atomText);
    m_parser->setFeedId(m_id);
    QThread *workerThread = new QThread(this);

    connect(workerThread, &QThread::started, m_parser, &FeedParser::beginParsing);
    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
    m_parser->moveToThread(workerThread);

    workerThread->start();
}

void Subscription::refresh()
{
    if (m_id.isEmpty()) {
        qWarning() << "Can't refresh with an empty feed id";
        return;
    }
    QMap<QString, QString> params;
    params.insert("n", "20");
    params.insert("ck", QString::number(QDateTime::currentMSecsSinceEpoch()));
    m_netMan->get(ApiHelper::atomGetRequest(m_accessToken, m_id, params), [this](QNetworkReply* reply) {
        handleNetworkReply(reply);
    });
}

void Subscription::fetchMore()
{
    QMap<QString, QString> params;
    params.insert("n", "20");
    params.insert("ck", QString::number(QDateTime::currentMSecsSinceEpoch()));
    QString continuationToken = getContinuationToken();
    if(!continuationToken.isEmpty()) {
        params.insert("c", continuationToken);
    }
    m_netMan->get(ApiHelper::atomGetRequest(m_accessToken, m_id, params), [this](QNetworkReply* reply) {
        handleNetworkReply(reply);
    });
}



void Subscription::parsingComplete()
{
    m_parser->thread()->exit();

    emit updated(this);
}

#include "subscription.moc"
