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

WebView {
    id: authWebView
    anchors.fill: parent

    signal authCompleted(string token)

    onLoadingChanged: {
        if(title.indexOf("Success code") >= 0) {
            //lets extract our success code, pass it back to the authObject and hide the webview
            var i = title.indexOf("=");
            var code = title.slice(i+1);
            authCompleted(code);
            authWebView.destroy();
        }
    }
}
