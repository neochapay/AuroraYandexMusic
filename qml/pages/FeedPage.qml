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

import QtMultimedia 5.5

import ru.neochapay.ourmusic 1.0

import "../components"
import "../components/FeedPage"

Page {
    id: mainPage

    Component.onCompleted: {
        landing.get()
    }

    Connections{
        target: rotor
        onStantionTracksReady: {
            if(ourMusic.isMyWave && currentPlayListModel.rowCount == 0) {
                rotor.postStantionFeedback(Rotor.RadioStarted, tracks[0]);
            }

            for(var i = 0; i < tracks.length; i++) {
                currentPlayListModel.push(tracks[i])
            }

            if(currentPlayListModel.rowCount > 0 && currentPlayListModel.currentIndex == -1) {
                currentPlayListModel.currentIndex = 0
            }
        }
    }

    Label{
        id: errorLabel
        visible: false
        anchors.centerIn: parent
        text: qsTr("Oops. We have a problem.")
    }

    Landing{
        id: landing
        onLandingBlocksReady: {
            landingBlockRepeater.model = blocks
            busyIndicator.visible = false
        }
    }

    SilicaFlickable {
        id: feedView
        anchors.fill: parent
        contentHeight: header.height + feedColumn.height

        PageHeader {
            id: header
            title: qsTr("You feed")
        }

        VerticalScrollDecorator {
            id: scroll
        }

        PullDownMenu {
            id: feedPullMenu
            MenuItem {
                text: qsTr("Profile")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("UserPage.qml"));
                }
            }
            MenuItem {
                text: qsTr("Search")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SearchPage.qml"));
                }
            }
        }

        BusyIndicator {
            id: busyIndicator
            running: visible
            visible: true
            anchors.centerIn: parent
        }

        Column{
            id: feedColumn
            anchors{
                top: header.bottom
                topMargin: Theme.paddingLarge
                left: parent.left
                leftMargin: Theme.paddingLarge
            }
            width: parent.width-Theme.paddingLarge*2
            height: childrenRect.height-Theme.paddingLarge*2

            visible: !busyIndicator.visible
            spacing: Theme.paddingLarge

            MyWavePlayer{
                id: myWavePlayer
            }

            Repeater{
                id: landingBlockRepeater
                width: parent.width

                delegate: Loader{
                    id: landingBlocksLoader
                    width: parent.width

                    Component.onCompleted: {
                        if(modelData.type == "personal-playlists") {
                            landingBlocksLoader.setSource("../components/FeedPage/PersonalPlaylistsBlock.qml",
                                                          {
                                                              "blockData": modelData
                                                          })
                        }

                        if(modelData.type == "new-releases") {
                            landingBlocksLoader.setSource("../components/FeedPage/NewReleasesBlock.qml",
                                                          {
                                                              "blockData": modelData
                                                          })
                        }
                    }

                }
            }
        }
    }
}
