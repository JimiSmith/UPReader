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


#ifndef ARTICLE_H
#define ARTICLE_H

#include <QtCore/QObject>
#include <QStringList>
#include <QDate>
#include <QSqlRecord>

class Article : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString content READ content NOTIFY updated)
    Q_PROPERTY(QString summary READ summary NOTIFY updated)
    Q_PROPERTY(QString title READ title NOTIFY updated)
    Q_PROPERTY(bool unread READ unread NOTIFY updated)
    Q_PROPERTY(QString articleDomainName READ articleDomainName NOTIFY updated)
public:
    static Article* fromRecord(QSqlRecord record);

    explicit Article(QObject* parent = 0);
    virtual ~Article();

    QStringList state() const;
    void setState(const QStringList &state);
    void addState(const QString &state);

    QStringList tags() const;
    void setTags(const QStringList &tags);
    void addTag(const QString &tag);

    QString title() const;
    void setTitle(const QString &title);

    QString author() const;
    void setAuthor(const QString &author);

    QString link() const;
    void setLink(const QString &link);

    QString content() const;
    void setContent(const QString &content);

    QDateTime published() const;
    void setPublished(const QDateTime &published);

    QDateTime updated() const;
    void setUpdated(const QDateTime &updated);

    QString articleDomainName() const;
    void setArticleDomainName(const QString &articleDomainName);

    QString contentType() const;
    void setContentType(const QString &contentType);

    QString summary() const;

    bool unread() const;

private:
    QStringList m_state;
    QStringList m_tags;
    QString m_title;
    QString m_author;
    QString m_link;
    QString m_articleDomainName;
    QString m_contentType;
    QString m_content;
    QString m_summary;
    QDateTime m_published;
    QDateTime m_updated;

signals:
    void updated();
};

#endif // SUBSCRIPTION_H
