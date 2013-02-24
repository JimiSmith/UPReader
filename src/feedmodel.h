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

#ifndef FEEDMODEL_H
#define FEEDMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QDebug>
#include <QtCore/QSignalMapper>

#include "manager.h"

class Subscription;

class FeedModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString accessToken READ getAccessToken WRITE setAccessToken)
	Q_PROPERTY(QString refreshToken READ getRefreshToken WRITE setRefreshToken)
	enum roles { titleRole = Qt::UserRole + 1, unreadRole, idRole };
public:
	FeedModel(QObject* parent = 0);
	virtual ~FeedModel();
	
	void setAccessToken(QString token) {
		m_accessToken = token;
		if(!m_accessToken.isEmpty()) {
			m_man->setAccessToken(m_accessToken);
			m_man->refreshSubList();
		}
	}
	QString getAccessToken() { return m_accessToken; }
	void setRefreshToken(QString token) { m_refreshToken = token; m_man->setRefreshToken(m_refreshToken);}
	QString getRefreshToken() { return m_refreshToken; }
	Q_INVOKABLE Subscription* getSubscription(int ind) { return m_subList.at(ind); }

    virtual QHash<int, QByteArray> roleNames() const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

private:
	QString m_accessToken;
	QString m_refreshToken;
	QList<Subscription*> m_subList;
	Manager* m_man;
	QSignalMapper* m_mapper;

private slots:
	void setSubList(QList<Subscription*> subList);
	void subUpdated(int row);

signals:
	void subListUpdated();
};

#endif // FEEDMODEL_H
