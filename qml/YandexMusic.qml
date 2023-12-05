import QtQuick 2.0
import QtMultimedia 5.5

import Sailfish.Silica 1.0

import Amber.Mpris 1.0
import ru.neochapay.yandexmusic 1.0

import "pages"
import "components"

ApplicationWindow {
    id: root
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
        onUserIDChanged: feedbackSender.userID = user.userID
    }

    FeedbackSender{
        id: feedbackSender

    }

    MusicFetcher{
        id: musicFetcher
        onTrackReady: {
            rootAudio.source = path
            if(currentPlayListModel.currentIndex > 0) {
                rootAudio.play()
            }
        }
    }

    CurrentPlayListModel{
        id: currentPlayListModel
        onCurrentIndexChanged: {
            musicFetcher.load(currentPlayListModel.getCurrentTrack())
        }
    }

    MainPlayer{
        id: mainPlayer
        visible: currentPlayListModel.rowCount > 0
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

    MprisPlayer {
        id: mprisPlayer

        property string artist: qsTr("Loading")
        property string song: qsTr("tags...")

        serviceName: "yandex-music"
        identity: "AuroraYandexMusic"
        supportedUriSchemes: ["file"]
        supportedMimeTypes: ["audio/x-wav", "audio/x-vorbis+ogg", "audio/mpeg"]


        canSeek: true

        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: true
        canPlay: true

        playbackStatus: (rootAudio.playbackState === MediaPlayer.PlayingState) ? Mpris.Playing : Mpris.Paused

        onArtistChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.Artist)] = [artist] // List of strings
            mprisPlayer.metadata = metadata
        }

        onSongChanged: {
            var metadata = mprisPlayer.metadata
            metadata[Mpris.metadataToString(Mpris.Title)] = song // String
            mprisPlayer.metadata = metadata
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
            ++playListModel.currentIndex
        }
        onPreviousRequested: {
            --playListModel.currentIndex
        }
    }
}
