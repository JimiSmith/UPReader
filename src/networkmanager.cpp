#include <QDebug>

#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    m_netMan(new QNetworkAccessManager(this))
{
    connect(m_netMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void NetworkManager::post(QNetworkRequest request, QByteArray body, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(post(request, body), complete);
}

void NetworkManager::get(QNetworkRequest request, std::function<void (QNetworkReply *)> complete)
{
    m_operations.insert(get(request), complete);
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
        auto func = m_operations.value(reply);
        if (func) {
            func(reply);
            m_operations.remove(reply);
        } else {
            emit requestComplete(reply);
        }
    }
    reply->deleteLater();
}
