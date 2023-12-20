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

Item {
    id: artistListItemDelegate
    width: parent.width
    height: Theme.itemSizeLarge

    signal clicked();

    property var artist

    Image{
        id: artistCover
        width: parent.height*0.8
        height: width

        anchors{
            left: parent.left
            leftMargin: parent.height*0.1
            top: parent.top
            topMargin: parent.height*0.1
        }

        source: artist.coverUri ? "https://"+artist.coverUri.replace("%%", "100x100") : "../img/person.svg"
        fillMode: Image.PreserveAspectFit
    }

    Label{
        id: artistTitle
        text: artist.name

        color: Theme.highlightColor
        font.bold: true

        anchors{
            bottom: parent.verticalCenter
            bottomMargin: parent.height*0.1
            left: artistCover.right
            leftMargin: parent.height*0.1
        }
    }

    MouseArea{
        anchors.fill: parent
        onClicked: artistListItemDelegate.clicked()
    }
}
