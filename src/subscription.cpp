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
#include "articlelist.h"
#include "sqlhelper.h"

Subscription::Subscription()
{
    Subscription("", "");
}

Subscription::Subscription(QString token, QString id, QObject* parent)
    : QObject(parent)
{
    m_accessToken = token;
    m_id = id;
    m_netMan = new QNetworkAccessManager(this);
    m_parser = new FeedParser();
    connect(m_parser, SIGNAL(doneParsing()), this, SLOT(parsingComplete()));
    connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinshed(QNetworkReply*)));
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

void Subscription::refresh()
{
    //to get the feed we need to make a get call to https://www.google.com/reader/atom/
    //followed by the feed id
    //important parameters are
    //n - number of items to return
    //ck - current timestamp
    //c - a string indicating where to continue from. Each list that gets sent has a
    //gr:continuation tag in it - the content of that goes here
    QString url = QString("https://www.google.com/reader/atom/%0?n=%1&ck=%2")
                  .arg(m_id).arg(QString::number(20)).arg(QDateTime::currentMSecsSinceEpoch());
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(m_accessToken).toUtf8());
    m_operations.insert(m_netMan->get(r), refreshOP);
}

void Subscription::fetchMore()
{
    QString url = QString("https://www.google.com/reader/atom/%0?n=%1&ck=%2")
                  .arg(m_id).arg(QString::number(20)).arg(QDateTime::currentMSecsSinceEpoch());
    QString continuationToken = getContinuationToken();
    if(!continuationToken.isEmpty()) {
        url.append(QString("&c=%0").arg(continuationToken));
    }
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(m_accessToken).toUtf8());
    m_operations.insert(m_netMan->get(r), getMoreOP);
}

void Subscription::replyFinshed(QNetworkReply* reply)
{
    QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!possibleRedirectUrl.isEmpty()) { //we're being redirected
        m_netMan->get(QNetworkRequest(possibleRedirectUrl));
    } else { //we're at the endpoint
        m_atomText = QString::fromUtf8(reply->readAll());
        m_parser->setFeedString(m_atomText);
        m_parser->setFeedId(m_id);
        QThread *workerThread = new QThread(this);

        connect(workerThread, &QThread::started, m_parser, &FeedParser::beginParsing);
        connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
        m_parser->moveToThread(workerThread);

        workerThread->start();
    }
    reply->deleteLater();
}

void Subscription::parsingComplete()
{
    m_parser->thread()->exit();

    emit updated(this);
}

#include "subscription.moc"
