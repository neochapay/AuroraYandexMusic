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

import "../components"

Page {
    id: albumPage
    property alias albumId: albumInfo.albumId

    AlbumInfo{
        id: albumInfo
        albumId: album.albumId

        Component.onCompleted: {
            albumInfo.get();
        }

        onAlbumReady: {
            mainData.model = album.tracks
            mainData.height = mainData.model.length * Theme.itemSizeLarge
            albumCover.source = "https://"+album.coverUri.replace("%%","1000x1000")
            artistName.text = album.title
        }
    }

    SilicaFlickable {
        id: albumView
        anchors.fill: parent
        contentHeight: coverWraper.height + mainData.height + playButtonItem.height + Theme.paddingMedium*4

        Item {
            id: coverWraper
            width: parent.width
            height: width

            Image{
                id: albumCover
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit

                Label{
                    id: artistName
                    width: parent.width

                    anchors{
                        bottom: parent.verticalCenter
                    }

                    color: Theme.highlightColor
                    font.bold: true
                    font.pixelSize: Theme.fontSizeExtraLarge
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        Item {
            id: playButtonItem
            width: parent.width
            height: Theme.itemSizeMedium

            anchors{
                top: coverWraper.bottom
                topMargin: Theme.paddingMedium
            }

            SvgIcon {
                id: littlePlayerPlayButton

                width: parent.height
                height: width
                anchors.centerIn: parent
                source: "../img/play.svg"

                onClicked: {
                    rootAudio.stop()
                    currentPlayListModel.clear()
                    for(var i = 0; i < mainData.model.length; i++) {
                        currentPlayListModel.push(mainData.model[i])
                    }
                    currentPlayListModel.currentIndex = 0
                }
            }
        }

        ListView{
            id: mainData
            width: parent.width - Theme.paddingMedium*2
            spacing: Theme.paddingMedium
            anchors{
                left: parent.left
                leftMargin: Theme.paddingMedium
                top: playButtonItem.bottom
                topMargin: Theme.paddingMedium
            }

            delegate: TrackListItemDelegate{
                track: modelData
            }
        }
    }
}
