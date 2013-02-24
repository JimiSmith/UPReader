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

#include "feedmodel.h"
#include "subscription.h"

FeedModel::FeedModel(QObject* parent)
	: QAbstractListModel(parent)
{
	m_man = new Manager(this);
	m_mapper = new QSignalMapper(this);
	connect(m_man, SIGNAL(updatedSubList(QList<Subscription*>)), this, SLOT(setSubList(QList<Subscription*>)));
    connect(m_mapper, SIGNAL(mapped(int)), this, SLOT(subUpdated(int)));
}

FeedModel::~FeedModel()
{

}

QHash<int, QByteArray> FeedModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[titleRole] = "title";
    roles[unreadRole] = "unread";
    roles[idRole] = "id";

    return roles;
}

QVariant FeedModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid()) return QVariant();

	switch(role) {
		case titleRole: {
			return m_subList.at(index.row())->getTitle();
		}
		case unreadRole: {
			return m_subList.at(index.row())->getUnread();
		}
		case idRole: {
			return m_subList.at(index.row())->getId();
		}
	}
	return QVariant();
}

int FeedModel::rowCount(const QModelIndex& parent) const
{
	return m_subList.length();
}

void FeedModel::setSubList(QList< Subscription* > subList)
{
	beginResetModel();
	m_subList = subList;
	for(int i = 0; i < m_subList.size(); ++i) {
		Subscription* s = m_subList.at(i);
		m_mapper->removeMappings(s);
		connect(s, SIGNAL(updated()), m_mapper, SLOT(map()));
		m_mapper->setMapping(s, i);
	}
	emit subListUpdated();
	endResetModel();
}

void FeedModel::subUpdated(int row)
{
	dataChanged(createIndex(row, 0), createIndex(row, 0));
}

#include "feedmodel.moc"
