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

import ru.neochapay.ourmusic 1.0

ListItem {
    id: trackListItemDelegate
    width: parent.width
    contentHeight: Theme.itemSizeLarge

    signal clicked();

    property var track

    Image{
        id: trackCover
        width: parent.height*0.8
        height: width

        anchors{
            left: parent.left
            leftMargin: parent.height*0.1
            top: parent.top
            topMargin: parent.height*0.1
        }

        source: "https://"+track.artists[0].coverUri.replace("%%", "100x100")
        fillMode: Image.PreserveAspectFit
    }

    Item{
        id: trackTitleWrapper
        width: parent.width - trackCover.width - playButton.width - parent.height*0.4
        height: parent.height/2
        anchors{
            bottom: parent.verticalCenter
            left: trackCover.right
            leftMargin: parent.height*0.1
        }

        Label{
            id: trackTitle
            text: track.title

            color: Theme.highlightColor
            font.bold: true

            width: parent.width
            height: parent.height
            clip: true

            anchors{
                left: parent.left
                leftMargin: parent.height*0.1
                verticalCenter: parent.verticalCenter
            }
        }
    }
    Item{
        id: artistTitleWraper
        width: parent.width - trackCover.width - playButton.width - parent.height*0.4
        height: parent.height/2
        anchors{
            top: parent.verticalCenter
            left: trackCover.right
            leftMargin: parent.height*0.1
        }

        Label{
            id: artistTitle
            text: track.artists[0].name

            width: parent.width
            clip: true

            anchors{
                left: parent.left
                leftMargin: parent.height*0.1
                verticalCenter: parent.verticalCenter
            }
        }
    }

    SvgIcon{
        id: playButton
        width: parent.height*0.6
        height: width

        source: "../img/play.svg"

        anchors{
            right: parent.right
            rightMargin: parent.height*0.2
            top: parent.top
            topMargin: parent.height*0.2
        }

        onClicked: {
            ourMusic.isMyWave = false
            rootAudio.stop()
            currentPlayListModel.clear();
            currentPlayListModel.push(track)
            currentPlayListModel.currentIndex = 0
        }
    }


    Separator {
        id: separator
        anchors {
            bottom: parent.bottom
            bottomMargin: -1
        }
        width: parent.width
        color: Theme.primaryColor
        horizontalAlignment: Qt.AlignHCenter
    }

    menu: ContextMenu {
        /*MenuItem {
            text: qsTr("Like")
        }
        MenuItem {
            text: qsTr("Download")
        }*/
        MenuItem {
            text: qsTr("Go to artist")
            onClicked: pageStack.push(Qt.resolvedUrl("../pages/ArtistPage.qml"), {
                                          artistId: track.artists[0].artistId});
        }
        MenuItem {
            text: qsTr("Go to album")
            onClicked: pageStack.push(Qt.resolvedUrl("../pages/AlbumPage.qml"), {
                                          albumId: track.albums[0].albumId});
        }
        /*MenuItem {
            text: qsTr("Play next")
        }*/
    }
}
