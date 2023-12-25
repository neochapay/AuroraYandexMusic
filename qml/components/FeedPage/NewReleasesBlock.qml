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

import "../"

Item {
    id: newReleasesBlock
    height: childrenRect.height
    clip: true
    property var blockData

    Label{
        id: newReleasesBlockTitle
        width: parent.width
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeLarge
        font.bold: true
        horizontalAlignment: Text.AlignLeft
        text: blockData.title

        anchors{
            top: parent.top
            topMargin: Theme.paddingLarge
        }
    }

    ListView{
        id: newReleasesListView
        width: parent.width

        anchors{
            top: newReleasesBlockTitle.bottom
            topMargin: Theme.paddingLarge
        }
        model: newReleasesBlock.blockData.entities
        orientation: ListView.Horizontal
        spacing: Theme.paddingLarge

        delegate: AlbumCoverFeed{
            id: newReleasesCoverFeed
            album: modelData
            onClicked: pageStack.push(Qt.resolvedUrl("../../pages/AlbumPage.qml"), {
                                          album: modelData});
        }
    }
}
