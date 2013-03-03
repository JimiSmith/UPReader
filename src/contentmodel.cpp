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
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

#include "contentmodel.h"
#include "article.h"
#include "articlelist.h"

ContentModel::ContentModel(QObject* parent)
    : QmlSqlTableModel(parent)
{
    m_subscription = 1;
    setTable("articles");
//    QSqlQuery q;
//    q.prepare("SELECT * FROM articles LIMIT 1");
//    setQuery(q);
}

ContentModel::~ContentModel()
{

}

void ContentModel::setSubscription(int sub)
{
    m_subscription = sub;
    updated();
}

void ContentModel::updated()
{
    setFilter(QString("subscription_id='%1'").arg(m_subscription));
    select();
}

void ContentModel::itemsAppended(int i)
{
//    setQuery();// TODO implement
//    beginInsertRows(QModelIndex(), i, m_allItems->articleList().size() - 1);
//    endInsertRows();
}

Article* ContentModel::getArticle(int ind)
{
    return Article::fromRecord(record(ind));
}

#include "contentmodel.moc"
