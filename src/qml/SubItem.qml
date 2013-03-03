import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

ListItem.Subtitled {
    property string itemTitle: ""
    property int unreadCount: -1
    property bool updating: false

    text: itemTitle
    progression: !updating
    enabled: progression
    subText: progression ? unreadCount + " unread item" + (unreadCount == 1 ? "" : "s") : "loading..."
    highlightWhenPressed: true
    ActivityIndicator {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
//                anchors.rightMargin: units.gu(2)
        running: updating
    }
}
