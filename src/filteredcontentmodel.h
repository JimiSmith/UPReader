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


#ifndef FILTEREDCONTENTMODEL_H
#define FILTEREDCONTENTMODEL_H

#include <QSortFilterProxyModel>

#include "contentmodel.h"

class Subscription;

class FilteredContentModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Subscription* subscription READ getSubscription WRITE setSubscription)
    Q_PROPERTY(QString currentFeed READ getCurrentFeed WRITE setCurrentFeed)

public:
    FilteredContentModel(QObject* parent = 0);
    virtual ~FilteredContentModel();

    Subscription* getSubscription() {
        return qobject_cast<ContentModel*>(sourceModel())->getSubscription();
    }
    void setSubscription(Subscription* sub) {
        qobject_cast<ContentModel*>(sourceModel())->setSubscription(sub);
    }

    QString getCurrentFeed() {
        return m_currentFeed;
    }
    void setCurrentFeed(QString feed);

    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

    virtual bool canFetchMore(const QModelIndex& parent) const {
        return false;
    }
// 	virtual bool canFetchMore(const QModelIndex& parent) const { return sourceModel()->canFetchMore(parent); }
    virtual void fetchMore(const QModelIndex& parent) {
        sourceModel()->fetchMore(parent);
    }

    Q_INVOKABLE Article* getArticle(int ind) {
        return qobject_cast<ContentModel*>(sourceModel())->getArticle(ind);
    }

private:
    QString m_currentFeed;

};

#endif // FILTEREDCONTENTMODEL_H
