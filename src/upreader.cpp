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

#include <QtNetwork/QNetworkCookieJar>
#include <QtCore/qdatetime.h>
#include <QNetworkRequest>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QJsonDocument>

#include "upreader.h"
#include "apihelper.h"
#include "networkmanager.h"

Auth::Auth(QObject* parent)
    : QObject(parent)
{
    m_netMan = new NetworkManager(this);
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
        emit showWebView(u);
    } else {
        getNewAccessToken();
    }
}

void Auth::authReceived(QString token)
{
    //once we have the token we need to post to https://accounts.google.com/o/oauth2/token
    //with the following params
    //client_id
    //client_secret
    //grant_type=authorization_code
    QMap<QString, QString> params;
    params.insert("client_id", "444103914762.apps.googleusercontent.com");
    params.insert("client_secret", "ElXjwd-komrQeunn6OM80HrB");
    params.insert("grant_type", "authorization_code");
    params.insert("redirect_uri", "urn:ietf:wg:oauth:2.0:oob");
    params.insert("code", token);
    m_netMan->accountPost("oauth2/token", params, [this](QNetworkReply *reply) {
        QByteArray json = reply->readAll();
        QJsonDocument sd = QJsonDocument::fromJson(json);
        QVariant result = sd.toVariant();
        QVariantMap m = result.toMap();
        m_refreshToken = m.value("refresh_token").toString();
        m_accessToken = m.value("access_token").toString();
        QSettings settings("mrsmith", "upreader");
        //lets save the access token
        settings.beginGroup("auth");
        settings.setValue("refreshtoken", m_refreshToken);
        settings.setValue("accesstoken", m_accessToken);
        settings.endGroup();
        m_netMan->apiGet("token", QMap<QString, QString>(), [this](QNetworkReply *reply) {
            QByteArray out = reply->readAll();
            QSettings settings("mrsmith", "upreader");
            settings.beginGroup("user");
            settings.setValue("token", QString(out));
            settings.endGroup();
        });
        m_netMan->apiGet("user-info", QMap<QString, QString>(), [this](QNetworkReply *reply) {
            QByteArray json = reply->readAll();
            QJsonDocument sd = QJsonDocument::fromJson(json);
            QVariant result = sd.toVariant();
            QVariantMap m = result.toMap();
            QString userId = m.value("userId").toString();
            QSettings settings("mrsmith", "upreader");
            settings.beginGroup("user");
            settings.setValue("id", userId);
            settings.endGroup();
            emit haveAccessToken();
        });
    });
}

void Auth::getNewAccessToken()
{
    QMap<QString, QString> params;
    params.insert("client_id", "444103914762.apps.googleusercontent.com");
    params.insert("client_secret", "ElXjwd-komrQeunn6OM80HrB");
    params.insert("grant_type", "refresh_token");
    params.insert("refresh_token", m_refreshToken);
    m_netMan->accountPost("oauth2/token", params, [this](QNetworkReply *reply) {
        QByteArray json = reply->readAll();
        QJsonDocument sd = QJsonDocument::fromJson(json);
        QVariant result = sd.toVariant();

        QVariantMap m = result.toMap();
        m_accessToken = m.value("access_token").toString();
        m_netMan->apiGet("token", QMap<QString, QString>(), [this](QNetworkReply *reply) {
            QByteArray out = reply->readAll();
            QSettings settings("mrsmith", "upreader");
            settings.beginGroup("user");
            settings.setValue("token", QString(out));
            settings.endGroup();
        });
        emit haveAccessToken();
    });
}

#include "upreader.moc"
