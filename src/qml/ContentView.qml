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
import UPReader 0.1

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
