import QtQuick 2.0
import Ubuntu.Components 0.1
import QGReader 0.1

Page {
    property var subscription

    tools: ToolbarActions {
        Action {
            objectName: "action"
            text: i18n.tr("Open in browser")

            onTriggered: {
            }
        }
    }

    Component.onCompleted: {
        contentModel.subscription = subscription;
    }

    title: subscription.title

    ListView {
        id: content
        anchors.fill: parent

        model: ContentModel {
            id: contentModel
        }
        delegate: StoryDelegate{}
    }
}
