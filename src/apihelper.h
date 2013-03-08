#ifndef APIHELPER_H
#define APIHELPER_H

#include <QMap>
#include <QString>
#include <QNetworkRequest>

class ApiHelper
{
public:
    static QNetworkRequest atomGetRequest(QString id, QMap<QString, QString> queryParams);
    static QNetworkRequest getSubscriptionList();
    static QNetworkRequest acquireAccessToken();
    static QNetworkRequest markArticleRead();
    static QString getParamString(QMap<QString, QString> queryParams);
    static QString getAccessToken();
private:
    static QNetworkRequest accountsPostRequest(QString endpoint);
    static QNetworkRequest apiGetRequest(QString endpoint, QMap<QString, QString> queryParams);
    static QNetworkRequest apiPostRequest(QString endpoint);
    static QString appendParamsToUrl(QString url, QMap<QString, QString> queryParams);
};

#endif // APIHELPER_H
