import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import Sailfish.WebView 1.0

import "../components/"

WebViewPage {
    id: loginPage
    WebView {
        id: loginView
        anchors.fill: parent
        url: "https://oauth.yandex.ru/authorize?response_type=token&client_id=23cabbbdc6cd418abb4b39c32c41195d"

        onUrlChanged: {
            var urlString = loginView.url.toString();
            console.log("HELLO STRING >>>>>>>>>>" + urlString)
            if(urlString.indexOf("access_token=") != -1) {
                auth.storeToken(urlString)
            }
        }
    }

    Connections {
        target: auth
        onAuthorized: {
            pageContainer.replace(Qt.resolvedUrl("MainPage.qml"))
        }
    }
}

