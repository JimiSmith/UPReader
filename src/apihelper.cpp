#include <QDebug>

#include "apihelper.h"

QString ApiHelper::appendParamsToUrl(QString url, QMap<QString, QString> queryParams)
{
    QString newUrl = QString(url);
    if (queryParams.size() > 0) {
        newUrl.append("?");
        foreach (QString key, queryParams.keys()) {
            QString value = queryParams.value(key);
            newUrl.append(QString("%0=%1&").arg(key).arg(QString(QUrl::toPercentEncoding(value))));
        }
        newUrl.chop(1);
    }
    return newUrl;
}

QNetworkRequest ApiHelper::apiGetRequest(QString accessToken, QString endpoint, QMap<QString, QString> queryParams)
{
    QString url = appendParamsToUrl(QString("https://www.google.com/reader/api/0/%0").arg(endpoint), queryParams);
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(accessToken).toUtf8());
    return r;
}

QNetworkRequest ApiHelper::accountsPostRequest(QString endpoint)
{
    QNetworkRequest r = QNetworkRequest(QString("https://accounts.google.com/o/%0").arg(endpoint));
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return r;
}

//to get the feed we need to make a get call to https://www.google.com/reader/atom/
//followed by the feed id
//important parameters are
//n - number of items to return
//ck - current timestamp
//c - a string indicating where to continue from. Each list that gets sent has a
//gr:continuation tag in it - the content of that goes here
QNetworkRequest ApiHelper::atomGetRequest(QString accessToken, QString id, QMap<QString, QString> queryParams)
{
    QString url = appendParamsToUrl(QString("https://www.google.com/reader/atom/%0")
                                    .arg(QString(QUrl::toPercentEncoding(id))), queryParams);
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(accessToken).toUtf8());

    return r;
}

QNetworkRequest ApiHelper::getSubscriptionList(QString accessToken)
{
    QMap<QString, QString> params;
    params.insert("output", "json");
    return apiGetRequest(accessToken, "subscription/list", params);
}

QNetworkRequest ApiHelper::acquireAccessToken()
{
    return accountsPostRequest("oauth2/token");
}
