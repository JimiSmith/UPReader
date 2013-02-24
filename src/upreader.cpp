/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Jimi Smith <smithj002@gmail.com>

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

#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkRequest>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QJsonDocument>

#include "upreader.h"
#include <QtCore/qdatetime.h>

Auth::Auth(QObject* parent)
	: QObject(parent)
{
	m_netMan = new QNetworkAccessManager(this);
	connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinshed(QNetworkReply*)));
}

Auth::~Auth()
{
}

void Auth::getAuth()
{
    QSettings settings("mrsmith", "upreader");
	//lets get the access token
	settings.beginGroup("auth");
	m_refreshToken = settings.value("refreshtoken", QString("")).toString();
	settings.endGroup();
	if(m_refreshToken.isEmpty()) {
		QUrl u = QUrl("https://accounts.google.com/o/oauth2/auth?client_id=444103914762.apps.googleusercontent.com&redirect_uri=urn:ietf:wg:oauth:2.0:oob&scope=https://www.google.com/reader/api/ https://www.google.com/reader/atom/&response_type=code");
		qDebug() << u;
		emit showWebView(u);
	} else {
		getNewAccessToken();
	}
}

void Auth::authReceived(QString token)
{
    qDebug() << "Getting AUTH";
	//once we have the token we need to post to https://accounts.google.com/o/oauth2/token
	//with the following params
	//client_id
	//client_secret
	//grant_type=authorization_code
	QString params = QString("client_id=%0&client_secret=%1&grant_type=authorization_code&redirect_uri=urn:ietf:wg:oauth:2.0:oob&code=%2")
	.arg("444103914762.apps.googleusercontent.com")
	.arg("ElXjwd-komrQeunn6OM80HrB").arg(token);
    QNetworkRequest r = QNetworkRequest(QUrl("https://accounts.google.com/o/oauth2/token"));
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    m_operations.insert(m_netMan->post(r, params.toUtf8()), authOP);
}

void Auth::getNewAccessToken()
{
	QString params = QString("client_id=%0&client_secret=%1&grant_type=refresh_token&refresh_token=%2")
	.arg("444103914762.apps.googleusercontent.com")
	.arg("ElXjwd-komrQeunn6OM80HrB").arg(m_refreshToken);
    QNetworkRequest r = QNetworkRequest(QUrl("https://accounts.google.com/o/oauth2/token"));
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    m_operations.insert(m_netMan->post(r, params.toUtf8()), refreshOP);
}

void Auth::replyFinshed(QNetworkReply* reply)
{
	QByteArray json = reply->readAll();
    qDebug() << json;
    QJsonDocument sd = QJsonDocument::fromJson(json);
    QVariant result = sd.toVariant();
	switch(m_operations.value(reply)) {
		case authOP: {
			QVariantMap m = result.toMap();
			m_refreshToken = m.value("refresh_token").toString();
			m_accessToken = m.value("access_token").toString();
            QSettings settings("mrsmith", "upreader");
			//lets save the access token
			settings.beginGroup("auth");
			settings.setValue("refreshtoken", m_refreshToken);
			settings.endGroup();
			emit haveAccessToken();
			break;
		}
		case refreshOP: {
			QVariantMap m = result.toMap();
			m_accessToken = m.value("access_token").toString();
			emit haveAccessToken();
			break;
		}
	}
	reply->deleteLater();
}

#include "upreader.moc"
