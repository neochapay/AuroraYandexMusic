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
    id: playlistCoverFeed
    property string title: "TITLE"
    property string destription: "Descrition"
    property string cover
    clip: true

    signal clicked()

    width: Theme.itemSizeHuge*1.5
    height: playlistCoverFeed.width + coverTitle.height + coverDescription.height + Theme.paddingMedium*4

    Component.onCompleted: calcHeigh()
    onHeightChanged: calcHeigh()

    function calcHeigh() {
        if(generatedPlayListView.height < playlistCoverFeed.height) {
            generatedPlayListView.height = playlistCoverFeed.height
        }
    }

    Image {
        id: coverImage
        width: parent.width
        height: width
        fillMode: Image.PreserveAspectCrop
        source: "https://" + cover.replace("%%", "200x200")
    }

    Label{
        id: coverTitle
        text: playlistCoverFeed.title
        anchors{
            top: coverImage.bottom
            topMargin: Theme.paddingMedium
        }
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeMedium
    }

    Label{
        id: coverDescription
        text: playlistCoverFeed.destription
        width: parent.width
        anchors{
            top: coverTitle.bottom
            topMargin: Theme.paddingMedium
        }
        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeSmall
        wrapMode: Text.WordWrap
    }

    MouseArea{
        anchors.fill: parent
        onClicked: playlistCoverFeed.clicked()
    }
}
