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
import UPReader 0.1
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

        delegate: ListItem.Subtitled {
            text: title
            progression: unread == -1 ? 0 : 1
            enabled: progression
            subText: progression ? unread + " unread item" + (unread == 1 ? "" : "s") : "loading..."
            highlightWhenPressed: true
            onClicked: {
                itemClicked(feedModel.getSubscription(index));
            }
            ActivityIndicator {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
//                anchors.rightMargin: units.gu(2)
                running: unread == -1 ? 1 : 0
            }
        }
    }
}
