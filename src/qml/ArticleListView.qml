/*
    A Google Reader client for Ubuntu Phone
    Copyright (C) 2013  Jimi Smith <smithj002@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem
import UPReader 0.1

Page {
    property var subscription

    tools: ToolbarActions {
        Action {
            objectName: "action"
            text: i18n.tr("Dummy")

            onTriggered: {
                contentModel.refresh();
            }
        }
    }

    title: subscription.title

    ListView {
        id: content
        anchors.fill: parent

        Component.onCompleted: {
            contentModel.subscription = subscription;
        }

        model: ContentModel {
            id: contentModel
        }

        delegate: ListItem.Subtitled {
            text: title
            subText: summary
            progression: true
            onClicked: {
                pageStack.push(Qt.resolvedUrl('ArticleView.qml'), {article: contentModel.getArticle(index)})
            }
            UbuntuShape {
                color: unread ? '#AEA79F' : '#DD4814'
                width: units.gu(1)
                height: units.gu(1)
                anchors {
                    left: parent.left
                    leftMargin: units.gu(-1.5)
                    topMargin: units.gu(0)
                }
            }
        }

    }
}
