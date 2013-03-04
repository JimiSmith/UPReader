#ifndef SQLHELPER_H
#define SQLHELPER_H

class SqlHelper
{
public:
    static void createTableIfNeeded(QString tableName, QStringList values);
    static bool addOrUpdateSubBatch(QVariantList subData);
    static bool addOrUpdateSub(QVariantMap subData);
    static bool setAllSubsToLoadingState();
};

#endif // SQLHELPER_H
