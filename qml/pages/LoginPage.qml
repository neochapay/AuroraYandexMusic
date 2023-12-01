/*
 * Copyright (C) 2023 Chupligin Sergey <neochapay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

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
            auth.parseUrl(loginView.url.toString());
        }
    }

    Connections {
        target: auth
        onTokenChanged: {
            if(auth.token.length > 0 ) {
                pageContainer.replace(Qt.resolvedUrl("FeedPage.qml"))
            }
        }
    }
}

