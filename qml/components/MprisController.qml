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
import Amber.Mpris 1.0
import QtMultimedia 5.5

MprisPlayer {
    id: mprisPlayer

    property var track

    serviceName: "yandex-music"
    identity: qsTr("Own Music")
    supportedUriSchemes: ["file"]
    supportedMimeTypes: ["audio/mpeg"]


    canSeek: true
    canControl: true

    canPause: true
    canPlay: true

    playbackStatus: (rootAudio.playbackState === MediaPlayer.PlayingState) ? Mpris.Playing : Mpris.Paused

    onTrackChanged: {
        var metadata = mprisPlayer.metadata
        metaData.title = track.title
        metaData.contributingArtist = track.artists[0].name
        metaData.artUrl = "https://"+track.albums[0].coverUri.replace("%%", "100x100")
        mprisPlayer.canGoNext = currentPlayListModel.currentIndex < currentPlayListModel.rowCount
        mprisPlayer.canGoPrevious = currentPlayListModel.currentIndex > 0
    }

    onPauseRequested: {
        rootAudio.pause()
    }
    onPlayRequested: {
        rootAudio.play()
    }
    onPlayPauseRequested: {
        if (rootAudio.playbackState === MediaPlayer.PlayingState) {
            rootAudio.pause()
        } else {
            rootAudio.play()
        }
    }
    onStopRequested: {
        rootAudio.stop()
    }

    onNextRequested: {
        rootAudio.stop()
        ++currentPlayListModel.currentIndex
    }
    onPreviousRequested: {
        --currentPlayListModel.currentIndex
    }
}
