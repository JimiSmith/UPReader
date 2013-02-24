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
