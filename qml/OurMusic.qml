import QtQuick 2.0
import QtMultimedia 5.5
import Sailfish.Silica 1.0
import ru.neochapay.ourmusic 1.0

import "pages"
import "components"

ApplicationWindow {
    id: ourMusic

    property bool isMyWave: false
    property bool saveToFile: false

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

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    User{
        id: user
        Component.onCompleted: if(auth.token.length > 0) {
                                   user.getAccountStatus();
                               }
        onUserIDChanged: {
            feedbackSender.userID = user.userID
        }
        onLikeActionFinished: {
            if(actionId == currentPlayListModel.getCurrentTrack().trackId && action == "remove") {
                rootAudio.stop()
                currentPlayListModel.currentIndex++
            }
        }
    }

    FeedbackSender{
        id: feedbackSender
    }

    Rotor{
        id: rotor
    }

    MusicFetcher{
        id: musicFetcher
        onTrackReady: {
            if(saveToFile) {
                rootAudio.source = path
                if(currentPlayListModel.currentIndex > -1) {
                    rootAudio.play()
                }
            }
        }
        onFinalUrlReady: {
            if(!saveToFile) {
                rootAudio.source = url
                if(currentPlayListModel.currentIndex > -1) {
                    rootAudio.play()
                }
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

            musicFetcher.load(currentPlayListModel.getCurrentTrack())
            if(ourMusic.isMyWave && currentPlayListModel.currentIndex == currentPlayListModel.rowCount-1) {
                var lastTrack = currentPlayListModel.getCurrentTrack()
                rotor.getStationTracks("user:onyourwave", lastTrack.trackId)
            }
        }
    }

    MainPlayer{
        id: mainPlayer
        visible: currentPlayListModel.rowCount > 0 && currentPlayListModel.currentIndex != -1
        onVisibleChanged: {
            if(mainPlayer.visible) {
                ourMusic.bottomMargin = mainPlayer.height
            } else {
                ourMusic.bottomMargin = 0
            }
        }
    }

    MediaPlayer{
        id: rootAudio
        onStopped: {
            if (rootAudio.status == MediaPlayer.EndOfMedia) {
                feedbackSender.sendFeedback(currentPlayListModel.getCurrentTrack(), rootAudio.duration, rootAudio.position)
                ++currentPlayListModel.currentIndex
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
