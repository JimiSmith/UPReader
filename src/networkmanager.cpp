#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    m_netMan(new QNetworkAccessManager(this))
{
    connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

QNetworkReply* NetworkManager::post(QNetworkRequest request, QByteArray body)
{
    return m_netMan->post(request, body);
}

QNetworkReply* NetworkManager::get(QNetworkRequest request)
{
    return m_netMan->get(request);
}

void NetworkManager::replyFinished(QNetworkReply *reply)
{
    QUrl possibleRedirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if(!possibleRedirectUrl.isEmpty()) { //we're being redirected
        m_netMan->get(QNetworkRequest(possibleRedirectUrl));
    } else { //we're at the endpoint
        emit requestComplete(reply);
    }
    reply->deleteLater();
}
