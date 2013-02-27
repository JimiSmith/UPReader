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

#include "contentmodel.h"
#include "article.h"
#include "articlelist.h"

ContentModel::ContentModel(QObject* parent): QAbstractListModel(parent)
{
    m_subscription = NULL;
}

ContentModel::~ContentModel()
{

}

QHash<int, QByteArray> ContentModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[titleRole] = "title";
    roles[contentRole] = "content";
    roles[authorRole] = "author";
    roles[readRole] = "unread";
    roles[summaryRole] = "summary";
    return roles;
}

QVariant ContentModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid() || index.row() >= m_allItems->articleList().count()) return QVariant();

    Article* entry = m_allItems->articleList().at(index.row());
    switch(role) {
    case titleRole: {
        return entry->title();
    }
    case contentRole: {
        return entry->content();
    }
    case authorRole: {
        return entry->author();
    }
    case readRole: {
        return entry->unread();
    }
    case summaryRole: {
        return entry->summary();
    }
    }
}

int ContentModel::rowCount(const QModelIndex& parent) const
{
    if (m_subscription != NULL) {
        ArticleList* feedData = m_subscription->getFeedData();
        return feedData->articleList().length();
    }
}

void ContentModel::setSubscription(Subscription* sub)
{
    beginResetModel();
    m_subscription = sub;
    connect(m_subscription, SIGNAL(updated()), this, SLOT(updated()));
    connect(m_subscription, SIGNAL(itemsAppended(int)), this, SLOT(itemsAppended(int)));
    updated();
    endResetModel();
}

void ContentModel::updated()
{
    beginResetModel();
    m_allItems = m_subscription->getFeedData();
    endResetModel();
}

void ContentModel::itemsAppended(int i)
{
    m_allItems = m_subscription->getFeedData();
    beginInsertRows(QModelIndex(), i, m_allItems->articleList().size() - 1);
    endInsertRows();
}

void ContentModel::fetchMore(const QModelIndex& parent)
{
    qDebug() << "fetching more";
    m_subscription->fetchMore();
}

Article* ContentModel::getArticle(int ind)
{
    return m_allItems->articleList().at(ind);
}

#include "contentmodel.moc"
