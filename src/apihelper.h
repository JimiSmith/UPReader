#ifndef APIHELPER_H
#define APIHELPER_H

#include <QMap>
#include <QString>
#include <QNetworkRequest>

class ApiHelper
{
public:
    static QString getAccessToken();
    static QString getParamString(QMap<QString, QString> queryParams);
    static QString appendParamsToUrl(QString url, QMap<QString, QString> queryParams);
    static QNetworkRequest atomGetRequest(QString id, QMap<QString, QString> queryParams);
    static QNetworkRequest accountsPostRequest(QString endpoint);
    static QNetworkRequest apiGetRequest(QString endpoint, QMap<QString, QString> queryParams);
    static QNetworkRequest apiPostRequest(QString endpoint);
};

#endif // APIHELPER_H
