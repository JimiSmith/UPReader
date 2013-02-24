import QtQuick 2.0
import QGReader 0.1
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

Page {
    title: i18n.tr("Subscription List")

    signal itemClicked(var subscription)

    ListView {
        id: feedList
        anchors.fill: parent
        cacheBuffer: 1000 //FIXME: Calculate proeprly
        model: FeedModel {
            id: feedModel
            accessToken: authObject.accessToken
            refreshToken: authObject.refreshToken
        }

        delegate: ListItem.Standard {
            text: title
            progression: unread == -1 ? 0 : 1
            onClicked: {
                itemClicked(feedModel.getSubscription(index));
            }
            ActivityIndicator {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: units.gu(2)
                running: unread == -1 ? 1 : 0
            }
        }
    }
}
