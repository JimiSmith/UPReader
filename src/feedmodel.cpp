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
#include <QtSql/QSqlRecord>

#include "feedmodel.h"
#include "subscription.h"

FeedModel::FeedModel(QObject* parent)
    : QmlSqlTableModel(parent)
{
    m_man = new Manager(this);
    m_mapper = new QSignalMapper(this);
    setTable("subscriptions");
    connect(m_man, SIGNAL(updateSubList()), this, SLOT(updateSubList()));
    connect(m_man, SIGNAL(updateSub(QString)), this, SLOT(updateSub(QString)));
}

FeedModel::~FeedModel()
{

}

void FeedModel::refresh()
{
    m_man->refreshSubList();
}

void FeedModel::updateSubList()
{
    select();
}

void FeedModel::updateSub(QString id)
{
    //find the row that was updated
    int row = -1;
    for(int i = 0; i < rowCount(); ++i) {
        QSqlRecord r = record(i);
        if (r.value("google_id") == id) {
            row = i;
            break;
        }
    }

    if (row >= 0) {
        selectRow(row);
    }
}

#include "feedmodel.moc"
