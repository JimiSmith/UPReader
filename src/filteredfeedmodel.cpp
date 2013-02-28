#include <QDebug>

#include "filteredfeedmodel.h"

FilteredFeedModel::FilteredFeedModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void FilteredFeedModel::setFilter(QString filter)
{
    if (filter == "unread") {
        setFilterRole(FeedModel::unreadRole);
    } else if (filter == "favourites") {
        setFilterRole(-1);
    } else {
        setFilterRole(-1);
    }
    invalidateFilter();
}

bool FilteredFeedModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QVariant filterData = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), filterRole());
    switch (filterRole()) {
    case FeedModel::unreadRole: {
            bool ok;
            int unreadCount = filterData.toInt(&ok);
            return ok && unreadCount > 0;
        }
    case -1: {
            return true;
        }
    }
    return false;
}
