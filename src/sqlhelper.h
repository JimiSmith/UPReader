#ifndef SQLHELPER_H
#define SQLHELPER_H

class SqlHelper
{
public:
    static void createTableIfNeeded(QString tableName, QStringList values);
    static bool addOrUpdateSubBatch(QVariantList subData);
    static bool addOrUpdateSub(QVariantMap subData);
    static bool setAllSubsToLoadingState();
    static QString firstSubToUpdate();
    static int subIdForGoogleId(QString googleId);
    static QString googleIdForSub(int subId);
    static bool updateUnreadCountForGoogleId(QString googleId, int unread);

    static bool addOrUpdateArticleBatch(QVariantList articleList);
    static bool addOrUpdateArticle(QVariantMap articleData);
};

#endif // SQLHELPER_H
