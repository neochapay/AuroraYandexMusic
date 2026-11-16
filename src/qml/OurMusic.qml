/*
 * Copyright (C) 2023-2024 Chupligin Sergey <neochapay@gmail.com>
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
import QtMultimedia 5.6
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.neochapay.ourmusic 1.0

import "pages"
import "components"

ApplicationWindow {
    id: ourMusic

    property bool isMyWave: false
    property Track currentTrack;
    property bool playOnStation: false
    property real savedPhoneVolume: -1
    property bool applyingStationVolume: false
    property string suppressStationTrackId: ""
    property bool syncingFromStation: false
    property alias stationDetectorItem: stantionDetector
    property alias stationControllerItem: stationController

    initialPage: {
        if (auth.token.length > 0) {
            return Qt.createComponent(Qt.resolvedUrl("pages/FeedPage.qml"))
        } else {
            return Qt.createComponent(Qt.resolvedUrl("pages/LoginPage.qml"))
        }
    }

    Auth{
        id: auth
    }

    StantionDetector{
        id: stantionDetector
    }

    StationController{
        id: stationController
        onStatusChanged: {
            if (stationController.status.length > 0) {
                console.log("StationController:", stationController.status)
                stationStatusNotification.body = stationController.status
                stationStatusNotification.publish()
            }
        }
        onRemotePlayingChanged: {
            if (stationController.remotePlaying) {
                rootAudio.stop()
            }
        }
        onRemoteStateChanged: {
            if (ourMusic.playOnStation && stationController.remoteVolume >= 0) {
                var currentVol = rootAudio.volume
                console.log("Station state volume update: station =", stationController.remoteVolume, "player =", rootAudio.volume)
                if (Math.abs(currentVol - stationController.remoteVolume) > 0.01) {
                    console.log("Applying station volume to player:", stationController.remoteVolume)
                    ourMusic.applyingStationVolume = true
                    rootAudio.volume = stationController.remoteVolume
                    ourMusic.applyingStationVolume = false
                }
            }
        }
        onRemoteTrackChanged: {
            if (!ourMusic.playOnStation) {
                return
            }
            var remoteId = stationController.remoteTrackId
            if (!remoteId || remoteId.length === 0) {
                return
            }
            if (ourMusic.suppressStationTrackId === remoteId) {
                ourMusic.suppressStationTrackId = ""
                return
            }
            var current = currentPlayListModel.getCurrentTrack()
            if (current && String(current.trackId) === remoteId) {
                return
            }
            for (var i = 0; i < currentPlayListModel.rowCount; i++) {
                var t = currentPlayListModel.getTrack(i)
                if (t && String(t.trackId) === remoteId) {
                    ourMusic.syncingFromStation = true
                    currentPlayListModel.currentIndex = i
                    ourMusic.syncingFromStation = false
                    return
                }
            }
            console.log("Remote station track not found in current playlist:", remoteId)
        }
    }

    Connections {
        target: Qt.application
        onAboutToQuit: {
            if (ourMusic.playOnStation) {
                stationController.stopRemotePlayback()
                stationController.disconnectFromStation()
            }
        }
    }

    Component.onDestruction: {
        if (playOnStation) {
            stationController.stopRemotePlayback()
            stationController.disconnectFromStation()
        }
    }

    Notification{
        id: stationStatusNotification
        appName: "OwnMusic"
        summary: qsTr("Station")
        previewSummary: qsTr("Station")
        previewBody: stationController.status
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    User{
        id: user
        Component.onCompleted: if(auth.token.length > 0) {
                                   user.getAccountStatus();
                               }
        onUserIDChanged: {
            feedbackSender.userID = user.userID
            user.loadLikedTracks()
        }
        onLikeActionFinished: {
            if(actionId == currentPlayListModel.getCurrentTrack().trackId && action == "remove") {
                rootAudio.stop()
                currentPlayListModel.currentIndex++
            }
            user.loadLikedTracks()
        }
    }

    FeedbackSender{
        id: feedbackSender
    }

    Rotor{
        id: rotor
    }

    Connections{
        target: currentTrack
        onTrackChanged:{
            if (ourMusic.playOnStation) {
                return
            }
            if(currentTrack && currentTrack === currentPlayListModel.getCurrentTrack() && currentTrack.downloaded) {
                rootAudio.source = currentTrack.filePath;
                rootAudio.play()
            }
        }
    }

    onPlayOnStationChanged: {
        if (!playOnStation) {
            if (savedPhoneVolume >= 0) {
                rootAudio.volume = savedPhoneVolume
                savedPhoneVolume = -1
            }
            var track = currentPlayListModel.getCurrentTrack();
            if (track && track.downloaded) {
                rootAudio.source = track.filePath;
                rootAudio.play()
            }
        } else {
            rootAudio.stop()
            if (savedPhoneVolume < 0) {
                savedPhoneVolume = rootAudio.volume
            }
            if (stationController.remoteVolume >= 0) {
                applyingStationVolume = true
                rootAudio.volume = stationController.remoteVolume
                applyingStationVolume = false
            }
        }
    }

    CurrentPlayListModel{
        id: currentPlayListModel
        onCurrentIndexChanged: {
            //send feedback
            var track = currentPlayListModel.getPrevTrack();
            if(track) {
                feedbackSender.sendFeedback(track, rootAudio.duration, rootAudio.position)
            }
            //send rotor feedback
            //TODO
            var tr = currentPlayListModel.getCurrentTrack()

            if(!tr) {
                console.warn("Track is null")
                return
            }

            currentTrack = tr;

            if(ourMusic.isMyWave && currentPlayListModel.currentIndex >= currentPlayListModel.rowCount-1) {
                rotor.getStationTracks("user:onyourwave", currentTrack.trackId)
            }

            if (ourMusic.playOnStation) {
                rootAudio.stop()
                if (!ourMusic.syncingFromStation) {
                    ourMusic.suppressStationTrackId = String(currentTrack.trackId)
                    var albumId = 0
                    if (currentTrack.albums && currentTrack.albums.length > 0) {
                        albumId = currentTrack.albums[0].albumId
                    }
                    stationController.playTrackByIds(parseInt(currentTrack.trackId), albumId)
                }
            } else if(currentTrack.downloaded) {
                rootAudio.source = currentTrack.filePath;
                rootAudio.play()
            } else {
                rootAudio.stop()
                currentTrack.download()
            }
        }
    }


    MainPlayer{
        id: mainPlayer
        visible: currentPlayListModel.rowCount > 0 && currentPlayListModel.currentIndex != -1
        stationDetector: stantionDetector
        stationController: stationController
        playOnStation: ourMusic.playOnStation
        onPlayOnStationChanged: ourMusic.playOnStation = playOnStation
        onVisibleChanged: {
            if(mainPlayer.visible) {
                //ourMusic.bottomMargin = mainPlayer.height
            } else {
                //ourMusic.bottomMargin = 0
            }
        }
    }

    MediaPlayer{
        id: rootAudio
        onVolumeChanged: {
            console.log("Player volume changed:", rootAudio.volume, "playOnStation:", ourMusic.playOnStation, "applyingStationVolume:", ourMusic.applyingStationVolume)
            if (ourMusic.playOnStation && !ourMusic.applyingStationVolume) {
                console.log("Syncing player volume to station:", rootAudio.volume)
                stationController.setRemoteVolume(rootAudio.volume)
            }
        }
        onStatusChanged: {
            if (rootAudio.status == MediaPlayer.EndOfMedia) {
                feedbackSender.sendFeedback(currentPlayListModel.getCurrentTrack(), rootAudio.duration, rootAudio.position)
                if(ourMusic.isMyWave && currentPlayListModel.currentIndex >= currentPlayListModel.rowCount-1) {
                    var track = currentPlayListModel.getCurrentTrack();
                    if(track) {
                        rotor.getStationTracks("user:onyourwave", track.trackId)
                    }
                } else if(currentPlayListModel.currentIndex < currentPlayListModel.rowCount - 1) {
                    ++currentPlayListModel.currentIndex
                }
            }
        }
        onPlaybackStateChanged: {
            if (rootAudio.playbackState == MediaPlayer.StoppedState && rootAudio.status == MediaPlayer.EndOfMedia) {
                if(ourMusic.isMyWave && currentPlayListModel.currentIndex >= currentPlayListModel.rowCount-1) {
                    var track = currentPlayListModel.getCurrentTrack();
                    if(track) {
                        rotor.getStationTracks("user:onyourwave", track.trackId)
                    }
                } else if(currentPlayListModel.currentIndex < currentPlayListModel.rowCount - 1) {
                    ++currentPlayListModel.currentIndex
                }
            }
        }
    }

    MprisController{
        id: mprisController
    }

    Connections{
        target: currentPlayListModel
        onCurrentIndexChanged: {
            mprisController.track = currentPlayListModel.getCurrentTrack();
        }
    }
}
