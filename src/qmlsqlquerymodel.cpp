#include <QtCore/QDebug>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>

#include "qmlsqlquerymodel.h"

QmlSqlQueryModel::QmlSqlQueryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QHash<int, QByteArray> QmlSqlQueryModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    for( int i = 0; i < record().count(); i++) {
        roles[Qt::UserRole + i + 1] = record().fieldName(i).toLatin1();
    }

    return roles;
}

QVariant QmlSqlQueryModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid()) return QVariant();
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}
