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
import QtWebKit 3.0
import UPReader 0.1
import Ubuntu.Components 0.1

MainView {
    id: applicationView
    applicationName: "UP Reader"
    width: units.gu(40)
    height: units.gu(75)

    property string qtwebkitdpr: "1.0"

    function showLoading() {
        loadingIndicator.running = true
    }

    function hideLoading() {
        loadingIndicator.running = false
    }

    function authCompleted(authToken) {
        authObject.authReceived(authToken);
        hideLoading();
    }

    function showWebAuth(authUrl) {
        showLoading();
        var authComponent = Qt.createComponent("WebAuth.qml");
        var authView = authComponent.createObject(applicationView, {url: authUrl});
        authView.authCompleted.connect(authCompleted);
    }

    function showSubList() {
        hideLoading();
        readerView.visible = true;
        readerView.refresh();
    }

    Auth {
        id: authObject
        onShowWebView: {
            showWebAuth(authUrl);
        }
        onHaveAccessToken: {
            showSubList();
        }
    }

    Component.onCompleted: {
        authObject.getAuth();
    }

    ReaderView {
        id: readerView
        anchors.fill: parent
        visible: false
    }

    ActivityIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        running: true
    }
}
