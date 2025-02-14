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
import QtMultimedia
import Sailfish.Silica 1.0
import Nemo.Notifications 1.0
import ru.neochapay.ourmusic 1.0

import "pages"
import "components"

ApplicationWindow {
    id: ourMusic

    property bool isMyWave: false
    property Track currentTrack;

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

    /*StantionDetector{
        id: stantionDetector
        onStantionChanged: {
            if(stantionDetector.valid) {
                stantionDetectNotification.publish()
            }
        }
    }*/

    /*Notification{
        id: stantionDetectNotification
        appName: "OwnMusic"
        appIcon: "/usr/share/ru.neochapay.ourmusic/qml/img/devices/"+ stantionDetector.stantionPlatform + ".svg"
        summary: "Connect to " + stantionDetector.stantionName
        body: "Do you want play music on " + stantionDetector.stantionName
    }*/

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
            if(currentTrack.downloaded) {
                rootAudio.source = currentTrack.filePath;
                rootAudio.play()
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

            currentTrack = currentPlayListModel.getCurrentTrack();
            if(ourMusic.isMyWave && currentPlayListModel.currentIndex == currentPlayListModel.rowCount-1) {
                rotor.getStationTracks("user:onyourwave", currentTrack.trackId)
            }
            if(currentTrack.downloaded) {
                rootAudio.source = currentTrack.filePath;
                rootAudio.play()
            } else {
                currentTrack.download(currentTrack)
            }
        }
    }


    MainPlayer{
        id: mainPlayer
        visible: currentPlayListModel.rowCount > 0 && currentPlayListModel.currentIndex != -1
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
        /*onStopped: {
            if (rootAudio.status == MediaPlayer.EndOfMedia) {
                feedbackSender.sendFeedback(currentPlayListModel.getCurrentTrack(), rootAudio.duration, rootAudio.position)
                ++currentPlayListModel.currentIndex
            }
        }*/
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
