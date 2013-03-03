#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>

#include "qmlsqltablemodel.h"

QmlSqlTableModel::QmlSqlTableModel(QObject *parent) :
    QSqlTableModel(parent)
{
}

QHash<int, QByteArray> QmlSqlTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    for( int i = 0; i < record().count(); i++) {
        roles[Qt::UserRole + i + 1] = record().fieldName(i).toLatin1();
    }

    return roles;
}

QVariant QmlSqlTableModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid()) return QVariant();
    QVariant value = QSqlTableModel::data(index, role);
    if(role < Qt::UserRole) {
        value = QSqlTableModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}
