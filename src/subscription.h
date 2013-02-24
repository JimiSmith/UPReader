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


#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include "feedparser.h"

class ArticleList;

class Subscription : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle NOTIFY updated)
    enum opType { refreshOP, getMoreOP };
    struct StoryContent {
        QString title;
        QString author;
        QString content;
        bool read;
    };
public:
    Subscription(QObject* parent = 0);
    explicit Subscription(QString token, QObject* parent = 0);
    virtual ~Subscription();

    void setId(QString id);
    QString getId();

    void setTitle(QString title);
    QString getTitle();

    void setCategories(QStringList cat);
    QStringList getCategories();

    void setSortId(QString sortid);
    QString getSortId();

    void setOldestItemTime(int time);
    int getOldestItemTime();

    void setUrl(QString url);
    QString getUrl();

    int getUnread() {
        return m_unread;
    }

    ArticleList* getFeedData() {
        return m_feedData;
    }

    bool canFetchMore() {
        return !m_continuation.isEmpty();
    }

private:
    QString m_accessToken;
    QString m_id;
    QString m_title;
    QStringList m_categories;
    QString m_sortid;
    int m_oldestItemTime;
    QString m_url;
    QString m_continuation;
    QString m_atomText;
    QNetworkAccessManager* m_netMan;
    QMap<QNetworkReply*, opType> m_operations;
    FeedParser* m_parser;
    ArticleList* m_feedData;
    int m_unread;

public slots:
    void fetchMore();

private slots:
    void replyFinshed(QNetworkReply* reply);
    void refresh();

signals:
    void updated();
    void itemsAppended(int);
};
Q_DECLARE_METATYPE(QList<Subscription*>)
Q_DECLARE_METATYPE(Subscription*)

#endif // SUBSCRIPTION_H
