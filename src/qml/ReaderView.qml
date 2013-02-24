import QtQuick 2.0
import Ubuntu.Components 0.1
import UPReader 0.1

PageStack {
    id: rootItem
    anchors.fill: parent

    Component.onCompleted: rootItem.push(page0)

    Page {
        id: page0
        title: "Subscriptions"
        anchors.fill: parent
        SubList {
            anchors.fill: parent
            onItemClicked: {
                rootItem.push(Qt.resolvedUrl('ContentView.qml'), {subscription: subscription});
            }
        }
    }

}
