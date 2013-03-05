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

QString SqlHelper::firstSubToUpdate()
{
    QSqlQuery query;
    query.exec("SELECT google_id FROM subscriptions WHERE needs_update=1 LIMIT 1");
    if (query.next()) {
        return query.value(0).toString();
    }

    return "";
}

int SqlHelper::subIdForGoogleId(QString googleId)
{
    QSqlQuery subQuery;
    subQuery.prepare("SELECT id FROM subscriptions WHERE google_id=:google_id");
    subQuery.bindValue(":google_id", googleId);
    int id = -1;
    if (subQuery.exec() && subQuery.next()) {
        id = subQuery.value(0).toInt();
    }

    return id;
}

bool SqlHelper::updateUnreadCountForGoogleId(QString googleId, int unread)
{
    QSqlQuery dataQuery;
    dataQuery.prepare("UPDATE subscriptions SET unread=:unread, needs_update=0 WHERE google_id=:google_id");
    dataQuery.bindValue(":unread", QString::number(unread));
    dataQuery.bindValue(":google_id", googleId);
    dataQuery.exec();
}

bool SqlHelper::addOrUpdateArticleBatch(QVariantList articleList)
{
    bool result = true;
    QSqlDatabase::database().transaction();
    foreach(QVariant m, articleList) {
        result = result && addOrUpdateArticle(m.toMap());
    }
    if (!result) {
        QSqlDatabase::database().rollback();
    } else {
        QSqlDatabase::database().commit();
    }

    return result;
}


bool SqlHelper::addOrUpdateArticle(QVariantMap articleData)
{
    QString google_id = articleData.value("google_id").toString();

    QSqlQuery existsQuery;
    existsQuery.prepare("SELECT count(google_id) FROM articles WHERE google_id=:google_id");
    existsQuery.bindValue(":google_id", google_id);
    bool existsRet = existsQuery.exec();

    if (!existsRet) {
        qWarning() << "Error with exists query" << existsQuery.lastError();
        return false;
    }

    bool exists = existsQuery.next() && existsQuery.value(0).toInt() > 0;

    QSqlQuery dataQuery;

    if (exists) {
        dataQuery.prepare("UPDATE articles SET title=:title, link=:link, "
                          "published=:published, updated=:updated, "
                          "article_domain_name=:article_domain_name, author=:author, "
                          "content=:content, unread=:unread, subscription_id=:subscription_id "
                          "WHERE google_id=:google_id");
    } else {
        dataQuery.prepare("INSERT INTO articles (google_id, title, link, published, updated, article_domain_name, author, content, unread, subscription_id) "
                          "VALUES (:google_id, :title, :link, :published, :updated, :article_domain_name, :author, :content, :unread, :subscription_id)");
    }

    dataQuery.bindValue(":google_id", google_id);
    dataQuery.bindValue(":title", articleData.value("title", ""));
    dataQuery.bindValue(":link", articleData.value("link", ""));
    dataQuery.bindValue(":published", articleData.value("published", ""));
    dataQuery.bindValue(":updated", articleData.value("updated", ""));
    dataQuery.bindValue(":article_domain_name", articleData.value("article_domain_name", ""));
    dataQuery.bindValue(":author", articleData.value("author", ""));
    dataQuery.bindValue(":content", articleData.value("content", ""));
    dataQuery.bindValue(":unread", articleData.value("unread", 0));
    dataQuery.bindValue(":subscription_id", articleData.value("subscription_id", 1));

    bool dataRet = dataQuery.exec();

    if (!dataRet) {
        qWarning() << "Error with update query" << dataQuery.lastError() << dataQuery.executedQuery();
        return false;
    }

    return true;
}
