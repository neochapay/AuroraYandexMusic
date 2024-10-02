/*
 * Copyright (C) 2023-2024 Chupligin Sergey <neochapay@gmail.com>
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

import ru.neochapay.ourmusic 1.0

import "../components"
import "../components/FeedPage"

Page {
    id: userPage

    SilicaFlickable {
        id: feedView
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("You profile")
        }

        VerticalScrollDecorator {
            id: scroll
        }

        Column{
            width: parent.width - Theme.paddingMedium * 2
            height: childrenRect.height

            anchors{
                top: header.bottom
                topMargin: Theme.paddingMedium
                left: parent.left
                leftMargin: Theme.paddingMedium
            }
            spacing: Theme.paddingMedium

            Button{
                text: qsTr("Downloaded tracks")
                width: parent.width
                onClicked: {
                    pageStack.replace(Qt.createComponent(Qt.resolvedUrl("UserDownloadedTracksPage.qml")))
                }
            }

            Button{
                text: qsTr("Liked tracks")
                width: parent.width
                onClicked: {
                    pageStack.replace(Qt.createComponent(Qt.resolvedUrl("UserLikedTracksPage.qml")))
                }
            }

            Button{
                text: qsTr("My playlists")
                width: parent.width
                onClicked: {
                    pageStack.replace(Qt.createComponent(Qt.resolvedUrl("UserPlaylistsPage.qml")))
                }
            }

            Button{
                text: qsTr("Logout")
                width: parent.width

                onClicked: {
                    auth.logout();
                    pageStack.replace(Qt.createComponent(Qt.resolvedUrl("LoginPage.qml")))
                }
            }
        }
    }
}
