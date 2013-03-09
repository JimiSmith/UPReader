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
#include <QTextDocument>
#include <QtGlobal>
#include <QSettings>

#include "article.h"
#include "networkmanager.h"
#include "apihelper.h"

Article::Article(QObject* parent)
    : QObject(parent)
{
    m_netMan = new NetworkManager(this);
}

Article::~Article()
{

}

QStringList Article::state() const
{
    return m_state;
}

void Article::setState(const QStringList &state)
{
    m_state = state;
}

void Article::addState(const QString &state)
{
    m_state.append(state);
}

QStringList Article::tags() const
{
    return m_tags;
}

void Article::setTags(const QStringList &tags)
{
    m_tags = tags;
}

void Article::addTag(const QString &tag)
{
    m_tags.append(tag);
}

QDateTime Article::updated() const
{
    return m_updated;
}

void Article::setUpdated(const QDateTime &updated)
{
    m_updated = updated;
}

QDateTime Article::published() const
{
    return m_published;
}

void Article::setPublished(const QDateTime &created)
{
    m_published = created;
}

QString Article::content() const
{
    return m_content;
}

void Article::setContent(const QString &content)
{
    m_content = content;
    QTextDocument doc;
    doc.setHtml(m_content);

    m_summary = doc.toPlainText();
    m_summary.replace("\n", "").replace("\t", " ").replace("\r", "");
    int truncatePos = qMin(80, m_summary.length() - 1);
    while (truncatePos < m_summary.length() - 1 && m_summary.at(truncatePos) != ' ') {
        truncatePos++;
    }
    m_summary.truncate(truncatePos);
    m_summary.append("...");
}

QString Article::link() const
{
    return m_link;
}

void Article::setLink(const QString &link)
{
    m_link = link;
}

QString Article::author() const
{
    return m_author;
}

void Article::setAuthor(const QString &author)
{
    m_author = author;
}

QString Article::title() const
{
    return m_title;
}

void Article::setTitle(const QString &title)
{
    m_title = title;
}

QString Article::articleDomainName() const
{
    return m_articleDomainName;
}

void Article::setArticleDomainName(const QString &articleDomainName)
{
    m_articleDomainName = articleDomainName;
}

QString Article::contentType() const
{
    return m_contentType;
}

void Article::setContentType(const QString &contentType)
{
    m_contentType = contentType;
}

QString Article::summary() const
{
    return m_summary;
}

bool Article::read() const
{
    return m_state.contains("read");
}

void Article::markRead(const bool read)
{
    QSettings settings("mrsmith", "upreader");
    settings.beginGroup("user");
    QString userId = settings.value("id").toString();
    settings.endGroup();
    QMap<QString, QString> params;
    params.insert("output", "json");
    params.insert("async", "true");
    params.insert("i", m_id);
    params.insert("s", m_subscriptionId);
    params.insert("a", QString("user/%0/state/com.google/read").arg(userId));
    //params.insert("T", ApiHelper::getAccessToken());
    qDebug() << "Marking read" << params;
    m_netMan->apiPost("edit-tag", params,
    [this](QNetworkReply *reply) {
        qDebug() << "Marked read" << reply->readAll();
    });
}

void Article::setRead(const bool r)
{
    if (r && !read()) {
        m_state.append("read");
    } else if (!r) {
        m_state.removeAll("read");
    }
}

QString Article::getId() const
{
    return m_id;
}

void Article::setId(const QString &id)
{
    m_id = id;
}

Article *Article::fromRecord(QSqlRecord record, QString subscription_id)
{
    Article *article = new Article();
    article->setAuthor(record.value("author").toString());
    article->setContent(record.value("content").toString());
    article->setLink(record.value("link").toString());
    article->setTitle(record.value("title").toString());
    article->setArticleDomainName(record.value("article_domain_name").toString());
    article->setRead(record.value("read").toBool());
    article->setId(record.value("google_id").toString());
    article->setSubscriptionId(subscription_id);
    return article;
}

QString Article::getSubscriptionId() const
{
    return m_subscriptionId;
}

void Article::setSubscriptionId(const QString &subscriptionId)
{
    m_subscriptionId = subscriptionId;
}
