#include <QDebug>
#include <QSettings>
#include <QDateTime>

#include "apihelper.h"

QString ApiHelper::appendParamsToUrl(QString url, QMap<QString, QString> queryParams)
{
    QString newUrl = QString(url);
    if (queryParams.size() > 0) {
        newUrl.append(QString("?%0").arg(getParamString(queryParams)));
        newUrl.append("&output=json");
    } else {
        newUrl.append("?output=json");
    }
    return newUrl;
}

QString ApiHelper::getAccessToken()
{
    QSettings settings("mrsmith", "upreader");
    //lets get the access token
    settings.beginGroup("auth");
    QString token = settings.value("accesstoken", QString("")).toString();
    settings.endGroup();

    return token;
}

QNetworkRequest ApiHelper::apiGetRequest(QString endpoint, QMap<QString, QString> queryParams)
{
    QString url = appendParamsToUrl(QString("https://www.google.com/reader/api/0/%0").arg(endpoint), queryParams);
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(getAccessToken()).toUtf8());
    return r;
}

QNetworkRequest ApiHelper::apiPostRequest(QString endpoint)
{
    QString url = QString("https://www.google.com/reader/api/0/%0?client=testong").arg(endpoint);
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(getAccessToken()).toUtf8());
    r.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
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
QNetworkRequest ApiHelper::atomGetRequest(QString id, QMap<QString, QString> queryParams)
{
    QString url = appendParamsToUrl(QString("https://www.google.com/reader/atom/%0")
                                    .arg(QString(QUrl::toPercentEncoding(id))), queryParams);
    QNetworkRequest r(url);
    r.setRawHeader("Authorization", QString("OAuth %0").arg(getAccessToken()).toUtf8());

    return r;
}

QString ApiHelper::getParamString(QMap<QString, QString> queryParams)
{
    QString newUrl = "";
    if (queryParams.size() > 0) {
        foreach (QString key, queryParams.keys()) {
            QString value = queryParams.value(key);
            newUrl.append(QString("%0=%1&").arg(key).arg(QString(QUrl::toPercentEncoding(value))));
        }
        newUrl.chop(1);
    }
    return newUrl;
}
