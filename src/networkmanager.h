#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = 0);
    QNetworkReply* post(QNetworkRequest request, QByteArray body);
    QNetworkReply* get(QNetworkRequest request);
    
signals:
    void requestComplete(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_netMan;

private slots:
    void replyFinished(QNetworkReply* reply);

};

#endif // NETWORKMANAGER_H
