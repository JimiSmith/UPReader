#include <QDebug>
#include "apihelper.h"

#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    m_netMan(new QNetworkAccessManager(this))
{
    connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void NetworkManager::apiPost(QString url, QMap<QString, QString> params, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(post(ApiHelper::apiPostRequest(url), ApiHelper::getParamString(params).toLatin1()), complete);
}

void NetworkManager::apiGet(QString url, QMap<QString, QString> params, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(get(ApiHelper::apiGetRequest(url, params)), complete);
}

void NetworkManager::atomGet(QString id, QMap<QString, QString> params, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(get(ApiHelper::atomGetRequest(id, params)), complete);
}

void NetworkManager::accountPost(QString url, QMap<QString, QString> params, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(post(ApiHelper::accountsPostRequest(url), ApiHelper::getParamString(params).toLatin1()), complete);
}

QNetworkReply* NetworkManager::post(QNetworkRequest request, QByteArray body)
{
    qDebug() << "POSTing" << request.url();
    return m_netMan->post(request, body);
}

QNetworkReply* NetworkManager::get(QNetworkRequest request)
{
    qDebug() << "GETing" << request.url();
    return m_netMan->get(request);
}

void NetworkManager::replyFinished(QNetworkReply *reply)
{
    qDebug() << "Got reply" << reply->url() << reply->error();
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error with request" << reply->url() << reply->error();
    } else {
        QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if(!possibleRedirectUrl.isEmpty()) { //we're being redirected
            m_netMan->get(QNetworkRequest(possibleRedirectUrl));
        } else { //we're at the endpoint
            auto func = m_operations.value(reply);
            if (func) {
                func(reply);
                m_operations.remove(reply);
            } else {
                emit requestComplete(reply);
            }
        }
    }
    reply->deleteLater();
}
