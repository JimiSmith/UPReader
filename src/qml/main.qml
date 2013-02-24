import QtQuick 2.0
import QtWebKit 3.0
import UPReader 0.1
import Ubuntu.Components 0.1

MainView {
    id: rootItem
    applicationName: "UP Reader"
    width: units.gu(40)
    height: units.gu(75)

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
        var authView = authComponent.createObject(rootItem, {url: authUrl});
        authView.authCompleted.connect(authCompleted);
    }

    function showSubList() {
        hideLoading();
        readerView.visible = true;
    }

    Auth {
        id: authObject
        onShowWebView: {
            showWebAuth(authUrl);
        }
        onHaveAccessToken: {
            console.log("Have access token " + authObject.accessToken);
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
