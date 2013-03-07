#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <functional>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = 0);
    void post(QNetworkRequest request, QByteArray body, std::function<void(QNetworkReply*)> complete);
    void get(QNetworkRequest request, std::function<void(QNetworkReply*)> complete);

signals:
    void requestComplete(QNetworkReply* reply);

private:
    QNetworkReply* post(QNetworkRequest request, QByteArray body);
    QNetworkReply* get(QNetworkRequest request);

    QNetworkAccessManager* m_netMan;
    QMap<QNetworkReply*, std::function<void(QNetworkReply*)> > m_operations;

private slots:
    void replyFinished(QNetworkReply* reply);

};

#endif // NETWORKMANAGER_H
