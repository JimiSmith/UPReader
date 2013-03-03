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


#ifndef FEEDPARSER_H
#define FEEDPARSER_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVariant>

class QDomElement;
class ArticleList;
class Article;

class FeedParser : public QObject
{
    Q_OBJECT

public:
    explicit FeedParser(QObject* parent = 0);
    virtual ~FeedParser();

    void setFeedString(QString atomText);
    void setTargetThread(QThread *target);

private:
    ArticleList* parseFeed();
    Article* parseEntry(QDomElement entry);
    QString unescape(QString s);

    QString m_atomText;
    QThread *m_targetThread;

public slots:
    void beginParsing();

signals:
    void doneParsing(ArticleList* feedList);
};

#endif // FEEDPARSER_H
