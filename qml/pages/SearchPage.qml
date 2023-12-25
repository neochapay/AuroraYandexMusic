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
    id: searchPage

    Search{
        id: search
        onSearchReady: {
            busyIndicator.visible = false
            artistsListView.model = result.artists
            albumListView.model = result.albums
            tracksListView.model = result.albums
        }
        onSearchStarted: busyIndicator.visible = true
    }

    BusyIndicator {
        id: busyIndicator
        running: visible
        visible: false
        anchors.centerIn: parent
    }

    SilicaFlickable {
        id: feedView
        anchors.fill: parent
        clip: true

        PageHeader {
            id: header
            title: qsTr("Search")
        }

        TextField{
            id: searchQuery
            width: parent.width
            onTextChanged: search.query = searchQuery.text
            anchors.top: header.bottom
            placeholderText: qsTr("Search")
        }

        //ARTISTS
        //ALBUMS
        //PLAYLISTS
        //TRACKS

        Column{
            id: mainColumn
            visible: !busyIndicator.visible
            width: parent.width
            anchors{
                top: searchQuery.bottom
            }

            Repeater{
                id: artistsListView
                width: parent.width

                delegate: ArtistListItemDelegate{
                    id: artistDelegate
                    artist: modelData
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("ArtistPage.qml"), { artistId : artistDelegate.artist.artistId })
                    }
                }
            }

            Repeater{
                id: albumListView
                width: parent.width

                delegate: AlbumListItemDelegate{
                    album: modelData
                    onClicked: pageStack.push(Qt.resolvedUrl("AlbumPage.qml"), { album : modelData })
                }
            }

            Repeater{
                id: tracksListView
                width: parent.width

                delegate: TrackListItemDelegate{
                    track: modelData
                }
            }
        }
    }
}
