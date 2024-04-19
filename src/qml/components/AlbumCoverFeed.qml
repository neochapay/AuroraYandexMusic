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

Item {
    id: albumCoverFeed
    property var album
    clip: true

    signal clicked()

    width: Theme.itemSizeHuge*1.5
    height: albumImage.height + albumTitle.height + albumDescription.height + Theme.paddingMedium*4

    Component.onCompleted: calcHeigh()
    onHeightChanged: calcHeigh()

    function calcHeigh() {
        if(newReleasesListView.height < albumCoverFeed.height) {
            newReleasesListView.height = albumCoverFeed.height
        }
    }

    Image {
        id: albumImage
        width: parent.width
        height: width
        fillMode: Image.PreserveAspectCrop
        source: "https://" + album.coverUri.replace("%%", "200x200")
    }

    Label{
        id: albumTitle
        text: album.title
        anchors{
            top: albumImage.bottom
            topMargin: Theme.paddingMedium
        }
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeMedium
    }

    Label{
        id: albumDescription
        text: album.artists[0].name
        width: parent.width
        anchors{
            top: albumTitle.bottom
            topMargin: Theme.paddingMedium
        }
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeSmall
        wrapMode: Text.WordWrap
    }

    MouseArea{
        anchors.fill: parent
        onClicked: albumCoverFeed.clicked()
    }
}
