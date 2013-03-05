#ifndef APIHELPER_H
#define APIHELPER_H

#include <QMap>
#include <QString>
#include <QNetworkRequest>

class ApiHelper
{
public:
    static QNetworkRequest atomGetRequest(QString accessToken, QString id, QMap<QString, QString> queryParams);
    static QNetworkRequest getSubscriptionList(QString accessToken);
    static QNetworkRequest acquireAccessToken();
private:
    static QNetworkRequest accountsPostRequest(QString endpoint);
    static QNetworkRequest apiGetRequest(QString accessToken, QString endpoint, QMap<QString, QString> queryParams);
    static QString appendParamsToUrl(QString url, QMap<QString, QString> queryParams);
};

#endif // APIHELPER_H
