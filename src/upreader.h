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

#ifndef UPREADER_H
#define UPREADER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkReply>

class Auth : public QObject
{
	Q_OBJECT
    Q_PROPERTY(QString accessToken READ getAccessToken NOTIFY haveAccessToken)
    Q_PROPERTY(QString refreshToken READ getRefreshToken NOTIFY haveAccessToken)
	enum opType { authOP, refreshOP };
public:
	explicit Auth(QObject* parent = 0);
	virtual ~Auth();

	QString getAccessToken() { return m_accessToken; }
	QString getRefreshToken() { return m_refreshToken; }

public slots:
	void getAuth();
	void authReceived(QString token);

private:
	void getNewAccessToken();
	
	QNetworkAccessManager* m_netMan;
	QMap<QNetworkReply*, opType> m_operations;
	QString m_accessToken;
	QString m_refreshToken;

private slots:
	void replyFinshed(QNetworkReply* reply);

signals:
	void showWebView(QUrl authUrl);
	void haveAccessToken();
};

#endif // UPREADER_H
