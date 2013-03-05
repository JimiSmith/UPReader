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


#ifndef MANAGER_H
#define MANAGER_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QMap>
#include <QtConcurrent>

#include "subscription.h"

class Manager : public QObject
{
    Q_OBJECT
    enum opType { listOP };
public:
    explicit Manager(QObject* parent = 0);
    virtual ~Manager();

    void setRefreshToken(QString refresh);
    void setAccessToken(QString access);

    void refreshSubList();

private:
    QNetworkAccessManager* m_netMan;
    QMap<QNetworkReply*, opType> m_operations;
    QString m_accessToken;
    QString m_refreshToken;
    QFutureWatcher<bool> m_watcher;

private slots:
    void replyFinshed(QNetworkReply* reply);
    void subUpdated(Subscription *sub);
    void syncSubscriptions();
    void refreshSubscriptions();

signals:
    void updateSub(QString id);
    void updateSubList();
};

#endif // MANAGER_H
