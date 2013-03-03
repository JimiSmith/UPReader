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

#ifndef UPREADERAPP_H
#define UPREADERAPP_H

#include <QApplication>
#include <QtSql/QSqlDatabase>

class QQuickView;

class UPReaderApp : public QApplication
{
    Q_OBJECT
public:
    explicit UPReaderApp(int& argc, char** argv);
    void initialize();
    int exec();

private slots:
    void onExit();

private:
    void setupDB();
    void createTableIfNeeded(QString tableName, QStringList values);

    QQuickView *m_view;
    QSqlDatabase m_appDB;

};

#endif // UPREADERAPP_H
