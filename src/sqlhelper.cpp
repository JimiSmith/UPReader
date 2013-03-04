#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStringList>
#include <QDebug>

#include "sqlhelper.h"

void SqlHelper::createTableIfNeeded(QString tableName, QStringList values)
{
    QSqlQuery tableQuery;
    QString sqlQuery = QString("SELECT count(name) FROM sqlite_master WHERE type = 'table' AND name = '%1'").arg(tableName);
    bool tableRet = tableQuery.exec(sqlQuery);

    if (!tableRet) {
        qWarning() << "There was an error creating the DB:" << tableQuery.lastError();
    }

    if (tableQuery.first() && tableQuery.value(0).toInt() > 0) {
        return;
    }

    QSqlQuery createQuery;
    QString sqlCreateQuery = QString("CREATE TABLE %1 (").arg(tableName);

    foreach (QString v, values) {
        sqlCreateQuery.append(v).append(",");
    }

    sqlCreateQuery.chop(1); //remove the final comma
    sqlCreateQuery.append(")");

    bool ret = createQuery.exec(sqlCreateQuery);

    if (!ret) {
        qWarning() << "There was an error creating the DB:" << createQuery.lastError();
    }
}

bool SqlHelper::addOrUpdateSubBatch(QVariantList subData)
{
    bool result = true;
    foreach(QVariant m, subData) {
        result = result && addOrUpdateSub(m.toMap());
    }

    return result;
}

bool SqlHelper::addOrUpdateSub(QVariantMap subData)
{
    QSqlDatabase::database().transaction();

    QString id = subData.value("id").toString();

    id.replace("?", "%3F");
    id.replace("=", "%3D");

    QSqlQuery existsQuery;
    existsQuery.prepare("SELECT count(google_id) FROM subscriptions WHERE google_id=:google_id");
    existsQuery.bindValue(":google_id", id);
    bool existsRet = existsQuery.exec();

    if (!existsRet) {
        QSqlDatabase::database().rollback();
        return false;
    }

    bool exists = existsQuery.next() && existsQuery.value(0).toInt() > 0;

    QSqlQuery dataQuery;

    if (exists) {
        dataQuery.prepare("UPDATE subscriptions SET title=:title, needs_update=1, url=:url, continuation=NULL WHERE google_id=:google_id");
    } else {
        dataQuery.prepare("INSERT INTO subscriptions (google_id, title, url, unread, continuation, needs_update) VALUES (:google_id, :title, :url, 0, NULL, 1)");
    }

    dataQuery.bindValue(":google_id", id);
    dataQuery.bindValue(":title", subData.value("title").toString());
    dataQuery.bindValue(":url", subData.value("htmlUrl").toString());

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();

    return true;
}

bool SqlHelper::setAllSubsToLoadingState()
{
    QSqlDatabase::database().transaction();
    QSqlQuery dataQuery;

    dataQuery.prepare("UPDATE subscriptions SET needs_update=1, unread=0");

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        QSqlDatabase::database().rollback();
        return false;
    }
    QSqlDatabase::database().commit();
}
