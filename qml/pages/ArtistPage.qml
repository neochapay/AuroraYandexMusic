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
    id: artistPage
    property int artistId
    property var artistData

    ArtistInfo{
        id: artistInfo
        artistId: artistPage.artistId

        Component.onCompleted: {
            getBriefInfo();
        }

        onGetBriefInfoArtistInfoReady: {
            busyIndicator.visible = false

            artistPage.artistData = artistData
            artistCover.source = "https://"+artistData.artist.coverUri.replace("%%","1000x1000")
            artistName.text = artistData.artist.name
            popularTracksRepeator.model = artistData.popularTracks
            popularTracksRepeator.height = popularTracksRepeator.model.length * Theme.itemSizeLarge
            albumsRepeator.model = artistData.albums
        }
    }

    BusyIndicator {
        id: busyIndicator
        running: visible
        visible: true
        anchors.centerIn: parent
    }

    SilicaFlickable {
        id: artistView
        anchors.fill: parent
        visible: !busyIndicator.visible
        contentHeight: coverWraper.height + mainData.height + Theme.paddingMedium*2

        Item {
            id: coverWraper
            width: parent.width
            height: width

            Image{
                id: artistCover
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

        Column{
            id: mainData
            width: parent.width - Theme.paddingMedium*2
            spacing: Theme.paddingMedium
            anchors{
                left: parent.left
                leftMargin: Theme.paddingMedium
                top: coverWraper.bottom
                topMargin: Theme.paddingMedium
            }

            Label{
                id: popularTracksLabel
                text: qsTr("Popular tracks")
            }

            ListView{
                id: popularTracksRepeator
                width: parent.width
                delegate: TrackListItemDelegate{
                    track: modelData
                }
            }

            Label{
                id: albumsLabel
                text: qsTr("Albums")
            }

            Repeater{
                id: albumsRepeator
                delegate: AlbumListItemDelegate{
                    album: modelData
                    onClicked: pageStack.push(Qt.resolvedUrl("AlbumPage.qml"), { albumId : modelData.albumId })
                }
            }

            /*Label{
                id: playlistLabel
                text: qsTr("Playlists")
            }

            Label{
                id: clipsLabel
                text: qsTr("Clips")
            }

            Label{
                id: alsoArtistsLabel
                text: qsTr("Also artists")
            }

            Label{
                id: concertsLabel
                text: qsTr("Concerts")
            }*/
        }

        //TRAKCS
        //ALBUMS
        //PLAYLISTS
        //CLIPS
        //LIKE ALSO
        //CONCERTS
        //LINKS

    }
}
