import QtQuick 2.0
import Sailfish.Silica 1.0

import QtMultimedia 5.6

Item{
    id: littlePlayer

    property alias cover: littlePlayerTrackCover.source
    property alias bgColor: background.color
    property var stationController: null
    property bool playOnStation: false


    Connections{
        target: rootAudio
        onStatusChanged: {
            if (littlePlayer.playOnStation) {
                return
            }
            if(rootAudio.status == MediaPlayer.NoMedia) {
                littlePlayerPlayButton.source = "../img/ban.svg"
            }

            if(rootAudio.status == MediaPlayer.Loaded) {
                littlePlayerPlayButton.source = "../img/play.svg"
            }
        }

        onPlaybackStateChanged: {
            if (littlePlayer.playOnStation) {
                return
            }
            if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                littlePlayerPlayButton.source = "../img/pause.svg"
            } else {
                littlePlayerPlayButton.source = "../img/play.svg"
            }
        }
    }

    Connections {
        target: stationController
        onRemoteStateChanged: {
            if (!littlePlayer.playOnStation) {
                return
            }
            littlePlayerPlayButton.source = stationController.remotePlaying ? "../img/pause.svg" : "../img/play.svg"
        }
    }

    Rectangle{
        id: payingStatus
        width: {
            var track = currentPlayListModel.getCurrentTrack();
            if (littlePlayer.playOnStation && stationController && stationController.remoteDuration > 0) {
                return parent.width * stationController.remoteProgress / stationController.remoteDuration
            }
            if(track && track.downloading) {
                return parent.width * track.downloadProgress
            }
            if(rootAudio.duration > 0) {
                return parent.width * rootAudio.position / rootAudio.duration
            }
            return 0
        }
        height: Theme.paddingSmall
        anchors{
            top: parent.top
            left: parent.left
        }
        color: littlePlayer.playOnStation ? Theme.highlightColor : Theme.highlightColor

    }

    Rectangle{
        id: background
        anchors.fill: parent
        color: "white"
        opacity: 1.0
    }

    Image{
        id: littlePlayerTrackCover
        height: parent.height - Theme.paddingSmall*2
        width: height

        anchors{
            top: parent.top
            topMargin: Theme.paddingSmall
            left: parent.left
            leftMargin: Theme.paddingSmall
        }

        fillMode: Image.PreserveAspectFit
    }


    Label{
        id: littlePlayerTitleLabel
        anchors{
            left: littlePlayerTrackCover.right
            leftMargin: Theme.paddingSmall
            bottom: parent.verticalCenter
        }
        width: parent.width - littlePlayerLikeButton.width - littlePlayerPlayButton.width - littlePlayerTrackCover.width - Theme.paddingSmall*4 - parent.height * 0.5
        height: parent.height / 2
        color: Theme.highlightColor
        verticalAlignment: Text.AlignVCenter
        clip: true
    }

    Label{
        id: littlePlayerArtistLabel
        anchors{
            left: littlePlayerTrackCover.right
            leftMargin: Theme.paddingSmall
            top: parent.verticalCenter
        }
        width: littlePlayerTitleLabel.width
        height: parent.height / 2
        color: Theme.primaryColor
        verticalAlignment: Text.AlignTop
        clip: true
    }

    SvgIcon {
        id: littlePlayerLikeButton
        width: parent.height * 0.5
        height: width

        anchors{
            right: littlePlayerPlayButton.left
            rightMargin: parent.height * 0.25
            verticalCenter: parent.verticalCenter
        }

        source: "../img/heart.svg"
        onClicked: user.like("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
    }


    SvgIcon {
        id: littlePlayerPlayButton

        width: parent.height * 0.5
        height: width
        anchors{
            right: parent.right
            rightMargin: parent.height * 0.25
            verticalCenter: parent.verticalCenter
        }
        source: (littlePlayer.playOnStation ? (stationController && stationController.remotePlaying) : (rootAudio.playbackState == MediaPlayer.PlayingState)) ? "../img/pause.svg" : "../img/play.svg"

        onClicked: {
            if (littlePlayer.playOnStation && stationController) {
                if (stationController.remotePlaying) {
                    stationController.pauseRemotePlayback()
                } else {
                    stationController.resumeRemotePlayback()
                }
            } else if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                rootAudio.pause()
            } else {
                rootAudio.play()
            }
        }
    }

    function applyRemoteTrackInfo() {
        if (!playOnStation || !stationController) {
            return
        }
        if (stationController.remoteTrackTitle && stationController.remoteTrackTitle.length > 0) {
            littlePlayerTitleLabel.text = stationController.remoteTrackTitle
        }
        if (stationController.remoteTrackArtist && stationController.remoteTrackArtist.length > 0) {
            littlePlayerArtistLabel.text = stationController.remoteTrackArtist
        }
        if (stationController.remoteTrackCover && stationController.remoteTrackCover.length > 0) {
            littlePlayer.cover = stationController.remoteTrackCover.replace("400x400", "100x100")
        }
    }

    Connections{
        target: currentPlayListModel
        onCurrentIndexChanged: {
            if(currentPlayListModel.rowCount > 0) {
                var track = currentPlayListModel.getCurrentTrack();
                if(track == null) {
                    return
                }

                littlePlayerTitleLabel.text = track.title
                littlePlayerArtistLabel.text = track.artists[0].name
                littlePlayer.cover = "https://"+track.albums[0].coverUri.replace("%%", "100x100")
                littlePlayer.bgColor = track.derivedColors.miniPlayer
            }
            applyRemoteTrackInfo()
        }
    }

    Connections {
        target: stationController
        onRemoteTrackChanged: applyRemoteTrackInfo()
    }

    onPlayOnStationChanged: applyRemoteTrackInfo()
}
