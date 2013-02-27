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

#include "article.h"

Article::Article(QObject* parent)
    : QObject(parent)
{
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

bool Article::unread() const
{
    return m_state.contains("read");
}
