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
                littlePlayerPlayButton.iconName = "icon-m-cancel"
            }

            if(rootAudio.status == MediaPlayer.Loaded) {
                littlePlayerPlayButton.iconName = "icon-m-play"
            }
        }

        onPlaybackStateChanged: {
            if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                littlePlayerPlayButton.iconName = "icon-m-pause"
            } else {
                littlePlayerPlayButton.iconName = "icon-m-play"
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

        fillMode: Image.PreserveAspectCrop
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
        text: mainPlayer.title
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
        text: mainPlayer.artist
        color: Theme.primaryColor
        verticalAlignment: Text.AlignTop
    }

    IconButton {
        id: littlePlayerLikeButton
        width: parent.height
        height: width

        anchors{
            right: littlePlayerPlayButton.left
        }

        icon.source: "image://theme/icon-m-like?" + (pressed
                                                     ? Theme.highlightColor
                                                     : Theme.primaryColor)
        onClicked: user.like("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
    }


    IconButton {
        id: littlePlayerPlayButton

        property string iconName: "icon-m-play"

        width: parent.height
        height: width
        anchors.right: parent.right
        icon.source: "image://theme/"+iconName+"?" + (pressed
                                                     ? Theme.highlightColor
                                                     : Theme.primaryColor)
        onClicked: if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                       rootAudio.pause()
                   } else {
                       rootAudio.play()
                   }
    }
}
