#ifndef QMLSQLTABLEMODEL_H
#define QMLSQLTABLEMODEL_H

#include <QSqlTableModel>

class QmlSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit QmlSqlTableModel(QObject *parent = 0);

    virtual QHash<int, QByteArray> roleNames() const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

#endif // QMLSQLTABLEMODEL_H
