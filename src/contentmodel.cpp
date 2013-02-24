/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Jimi Smith <smithj002@gmail.com>

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

ContentModel::ContentModel(QObject* parent): QAbstractListModel(parent)
{
    m_subscription = NULL;
}

ContentModel::~ContentModel()
{
	
}

QHash<int, QByteArray> ContentModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[titleRole] = "title";
    roles[contentRole] = "content";
    roles[authorRole] = "author";
    roles[readRole] = "read";
    return roles;
}

QVariant ContentModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid() || index.row() >= m_allItems.count()) return QVariant();

	QVariantMap entry = m_allItems.at(index.row()).toMap();
	switch(role) {
		case titleRole: {
            return entry.value("titleText");
		}
		case contentRole: {
            return entry.value("contentText");
		}
		case authorRole: {
            return entry.value("author");
		}
		case readRole: {
            return true;
		}
	}
}

int ContentModel::rowCount(const QModelIndex& parent) const
{
    if (m_subscription != NULL) {
        QVariantMap feedData = m_subscription->getFeedData();
        if (!feedData.value("entries").isNull()) {
            return feedData.value("entries").toList().length();
        }

        return 0;
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
    m_allItems = m_subscription->getFeedData().value("entries").toList();
	endResetModel();
}

void ContentModel::itemsAppended(int i)
{
    m_allItems = m_subscription->getFeedData().value("entries").toList();
	qDebug() << "old size:" << i << "new size:" << m_allItems.size();
	beginInsertRows(QModelIndex(), i, m_allItems.size() - 1);
	endInsertRows();
}

void ContentModel::fetchMore(const QModelIndex& parent)
{
	qDebug() << "fetching more";
	m_subscription->fetchMore();
}

#include "contentmodel.moc"
