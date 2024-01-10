import QtQuick 2.0
import Sailfish.Silica 1.0

import QtMultimedia 5.5

Item{
    id: littlePlayer

    property alias cover: littlePlayerTrackCover.source
    property alias bgColor: background.color


    Connections{
        target: rootAudio
        onStatusChanged: {
            if(rootAudio.status == MediaPlayer.NoMedia) {
                littlePlayerPlayButton.source = "../img/ban.svg"
            }

            if(rootAudio.status == MediaPlayer.Loaded) {
                littlePlayerPlayButton.source = "../img/play.svg"
            }
        }

        onPlaybackStateChanged: {
            if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                littlePlayerPlayButton.source = "../img/pause.svg"
            } else {
                littlePlayerPlayButton.source = "../img/play.svg"
            }
        }
    }

    Rectangle{
        id: payingStatus
        width: parent.width*rootAudio.position/rootAudio.duration
        height: Theme.paddingSmall
        anchors{
            top: parent.top
            left: parent.left
        }
        color: Theme.highlightColor

    }

    Rectangle{
        id: background
        anchors.fill: parent
        color: "white"
        opacity: 0.2
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
        width: parent.width - littlePlayerLikeButton.width - littlePlayerPlayButton.width - littlePlayerTrackCover.width - Theme.paddingSmall*2
        height: parent.height / 2
        color: Theme.highlightColor
        verticalAlignment: Text.AlignVCenter
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
        source: "../img/play.svg"

        onClicked: if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                       rootAudio.pause()
                   } else {
                       rootAudio.play()
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
        }
    }
}
