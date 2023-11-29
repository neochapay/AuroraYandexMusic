import QtQuick 2.0
import QtMultimedia 5.5

import Sailfish.Silica 1.0

import Amber.Mpris 1.0
import org.ilyavysotsky.yasailmusic 1.0

import "pages"

ApplicationWindow {
    id: root
    initialPage: {
        if (auth.token.length > 0) {
            return Qt.createComponent(Qt.resolvedUrl("pages/MainPage.qml"))
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

    }

    PlaylistModel{
        id: playListModel
        onCurrentIndexChanged: {
            mprisPlayer.song = playListModel.get(currentIndex).trackName
            mprisPlayer.artist = playListModel.get(currentIndex).artistName
            playListModel.sendFeedback("trackFinished")

            fileCacher.artistId = playListModel.get(currentIndex).artistId
            fileCacher.trackId = playListModel.get(currentIndex).trackId
            fileCacher.saveToCache();
        }
    }

    SearchModel{
        id: searchModel
        onCurrentIndexChanged: {
            mprisPlayer.song = searchModel.get(currentIndex).trackName
            mprisPlayer.artist = searchModel.get(currentIndex).artistName
            searchModel.sendFeedback("trackFinished")

            fileCacher.artistId = searchModel.get(currentIndex).artistId
            fileCacher.trackId = searchModel.get(currentIndex).trackId
            fileCacher.saveToCache();
        }
    }

    MediaPlayer{
        id: rootAudio
        onStopped: {
            if (rootAudio.status == MediaPlayer.EndOfMedia) {
                console.log("Track finished")
                ++playListModel.currentIndex
            }
        }
    }

    Cacher{
        id: fileCacher
        onFileSaved: {
            rootAudio.source = path
            rootAudio.play();
        }
    }

    MprisPlayer {
        id: mprisPlayer

        property string artist: qsTr("Loading")
        property string song: qsTr("tags...")

        serviceName: "yandex-music"
        identity: "YaSailMusic"
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
