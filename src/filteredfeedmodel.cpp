#include <QDebug>

#include "filteredfeedmodel.h"

FilteredFeedModel::FilteredFeedModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void FilteredFeedModel::setFilter(QString filter)
{
    m_filter = filter;
    if (sourceModel() == NULL) {
        return;
    }
    QHash<int, QByteArray> roles = ((FeedModel*)sourceModel())->roleNames();
    foreach(QByteArray v, roles.values()) {
        if (QString(v) == filter) {
            setFilterRole(roles.key(v));
            break;
        }
    }
    invalidateFilter();
}

void FilteredFeedModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);
    // set the filter again if the source model changes
    setFilter(m_filter);
}

bool FilteredFeedModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QVariant filterData = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), filterRole());
    if (m_filter == "unread") {
        bool ok;
        int unreadCount = filterData.toInt(&ok);
        return ok && unreadCount > 0;
    }
    return true;
}
