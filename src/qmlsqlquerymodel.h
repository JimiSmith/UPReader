// Shamelessly borrowed from http://qt-project.org/wiki/How_to_use_a_QSqlQueryModel_in_QML
#ifndef QMLSQLQUERYMODEL_H
#define QMLSQLQUERYMODEL_H

#include <QSqlQueryModel>

class QmlSqlQueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit QmlSqlQueryModel(QObject *parent = 0);

    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

#endif // QMLSQLQUERYMODEL_H
