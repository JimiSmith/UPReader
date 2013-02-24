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

#include "subscription.h"
#include <QtCore/qdatetime.h>

Subscription::Subscription() {
    Subscription("", 0);
}

Subscription::Subscription(QString token, QObject* parent)
: QObject(parent)
{
	m_accessToken = token;
	m_netMan = new QNetworkAccessManager(this);
	m_parser = new FeedParser(this);
    m_unread = -1;
	connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinshed(QNetworkReply*)));
}

Subscription::~Subscription()
{

}

void Subscription::setId(QString id)
{
	m_id = id;
	m_id.replace("?", "%3F");
	m_id.replace("=", "%3D");
	refresh();
}

QString Subscription::getId()
{
	return m_id;
}

void Subscription::setTitle(QString title)
{
	m_title = title;
}

QString Subscription::getTitle()
{
	return m_title;
}

void Subscription::setSortId(QString sortid)
{
	m_sortid = sortid;
}

QString Subscription::getSortId()
{
	return m_sortid;
}

void Subscription::setCategories(QStringList cat)
{
	m_categories = cat;
}

QStringList Subscription::getCategories()
{
	return m_categories;
}

void Subscription::setOldestItemTime(int time)
{
	m_oldestItemTime = time;
}

int Subscription::getOldestItemTime()
{
	return m_oldestItemTime;
}

void Subscription::setUrl(QString url)
{
	m_url = url;
}

QString Subscription::getUrl()
{
	return m_url;
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
	m_unread = -1;
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
	if(!m_continuation.isEmpty()) {
		url.append(QString("&c=%0").arg(m_continuation));
    }
	QNetworkRequest r(url);
	r.setRawHeader("Authorization", QString("OAuth %0").arg(m_accessToken).toUtf8());
	m_operations.insert(m_netMan->get(r), getMoreOP);
	m_continuation.clear();
}

void Subscription::replyFinshed(QNetworkReply* reply)
{
	QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
	if(!possibleRedirectUrl.isEmpty()) { //we're being redirected
		m_netMan->get(QNetworkRequest(possibleRedirectUrl));
	} else { //we're at the endpoint
		switch(m_operations.value(reply)) {
            case refreshOP: {
				m_atomText = QString::fromUtf8(reply->readAll());
				m_feedData = m_parser->parseFeed(m_atomText);
				m_continuation = m_feedData.value("cont").toString();
				m_unread = 0;
				foreach(QVariant v, m_feedData.value("entries").toList()) {
					QVariantMap m = v.toMap();
					if(!m.value("state").toList().contains("read")) {
						m_unread++;
					}
				}
				emit updated();
				break;
			}
			case getMoreOP: {
				QString r = QString::fromUtf8(reply->readAll());
				int i = m_feedData.value("entries").toList().size();
				QVariantMap feedData = m_parser->parseFeed(r);
				m_feedData["cont"] = feedData.value("cont");
				QVariantList l = m_feedData.value("entries").toList();
				l.append(feedData.value("entries").toList());
				m_feedData["entries"] = l;
				m_continuation = m_feedData.value("cont").toString();
				emit itemsAppended(i);
				break;
			}
		}
	}
	reply->deleteLater();
}

#include "subscription.moc"
