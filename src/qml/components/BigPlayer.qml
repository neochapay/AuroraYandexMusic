import QtQuick 2.0
import Sailfish.Silica 1.0

import QtMultimedia 5.5

Rectangle{
    id: bigPlayer
    width: parent.width
    height: mainPlayer.parent.height
    color: "black"
    visible: !littlePlayer.visible

    Connections{
        target: currentPlayListModel
        onCurrentIndexChanged: {
            if(currentPlayListModel.rowCount > 0) {
                var track = currentPlayListModel.getCurrentTrack();
                if(track == null) {
                    return
                }
                bigPlayerTitleLabel.text = track.title
                bigPlayerArtistLabel.text = track.artists[0].name
                if(track.albums[0].coverUri != "") {
                    bigPlayerTrackCover.source = "https://"+track.albums[0].coverUri.replace("%%", "400x400")
                }
                if(track.artists[0].coverUri != "") {
                    bigPlayerInfoItemArtistCover.source = "https://"+track.artists[0].coverUri.replace("%%", "100x100")
                }
            }
        }
    }

    IconButton {
        id: bigPlayerCloseButton
        width: Theme.itemSizeMedium
        height: width

        anchors{
            top: parent.top
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        icon.source: "image://theme/icon-m-cancel?#FFFFFF"
        onClicked: mainPlayer.height = Theme.itemSizeLarge
    }

    IconButton {
        id: bigPlayerPlaylistButton
        width: Theme.itemSizeMedium
        height: width

        anchors{
            top: parent.top
            topMargin: Theme.paddingLarge
            right: parent.right
            rightMargin: Theme.paddingLarge
        }

        icon.source: "image://theme/icon-m-menu?#FFFFFF"
        //onClicked: pageStack.push(Qt.resolvedUrl("pages/CurrentPlaylistPage.qml"))
    }

    Image{
        id: bigPlayerTrackCover
        height: width
        width: bigPlayer.width - Theme.paddingLarge*2

        anchors{
            top: bigPlayerPlaylistButton.bottom
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        fillMode: Image.PreserveAspectFit
    }

    Item {
        id: bigPlayerInfoItem
        height: Theme.itemSizeLarge
        width: bigPlayer.width - Theme.paddingLarge*2

        anchors{
            top: bigPlayerTrackCover.bottom
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        Image {
            id: bigPlayerInfoItemArtistCover
            width: parent.height
            height: width

            anchors{
                top: parent.top
                left: parent.left
            }

            fillMode: Image.PreserveAspectFit

            MouseArea{
                anchors.fill: parent
                onClicked: pageStack.push(Qt.resolvedUrl("../pages/ArtistPage.qml", { artist : track.artists[0] }))
            }
        }

        Label{
            id: bigPlayerTitleLabel
            anchors{
                left: bigPlayerInfoItemArtistCover.right
                leftMargin: Theme.paddingSmall
                bottom: parent.verticalCenter
            }
            width: parent.width - bigPlayerTrackCover.width - bigPlayerinfoButton.width - Theme.paddingSmall*2
            height: parent.height / 2
            color: Theme.highlightColor
            verticalAlignment: Text.AlignVCenter
        }

        Label{
            id: bigPlayerArtistLabel
            anchors{
                left: bigPlayerInfoItemArtistCover.right
                leftMargin: Theme.paddingSmall
                top: parent.verticalCenter
            }
            width: bigPlayerTitleLabel.width
            height: parent.height / 2
            color: Theme.primaryColor
            verticalAlignment: Text.AlignTop
        }

        IconButton {
            id: bigPlayerinfoButton
            width: parent.height
            height: width

            anchors{
                top: parent.top
                right: parent.right
            }

            icon.source: "image://theme/icon-m-about?#FFFFFF"
            //onClicked: pageStack.push(Qt.resolvedUrl("pages/AboutTrackPage.qml"))
        }
    }

    ProgressBar{
        id: bigPlayerProgressBar
        height: Theme.itemSizeLarge
        width: bigPlayer.width - Theme.paddingLarge*2

        anchors{
            top: bigPlayerInfoItem.bottom
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }
        minimumValue: 0
        maximumValue: rootAudio.duration
        value: rootAudio.position

        MouseArea{
            enabled: rootAudio.seekable
            anchors.fill: parent
            onClicked: {
                var pos = (mouseX - bigPlayerProgressBar.x)/bigPlayerProgressBar.width
                rootAudio.seek(pos * rootAudio.duration)
            }
        }
    }

    Item {
        id: bigPlayerControlsItem
        height: Theme.itemSizeLarge
        width: bigPlayer.width - Theme.paddingLarge*2

        anchors{
            top: bigPlayerProgressBar.bottom
            topMargin: Theme.paddingLarge
            left: parent.left
            leftMargin: Theme.paddingLarge
        }

        SvgIcon{
            id: bigPlayerDislikeButton
            width: parent.height*0.6
            height: width
            defaultColor: "white"
            activeColor: "deepskyblue"

            source: "../img/heart-crack.svg"

            anchors{
                right: bigPlayerBackwardButton.left
                rightMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            onClicked: user.dislike("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
        }

        SvgIcon{
            id: bigPlayerBackwardButton
            width: parent.height*0.6
            height: width
            defaultColor: "white"
            activeColor: "deepskyblue"

            source: "../img/backward.svg"

            anchors{
                right: bigPlayerPlayButton.left
                rightMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            onClicked: currentPlayListModel.currentIndex--
        }

        SvgIcon{
            id: bigPlayerPlayButton
            width: parent.height
            height: width
            defaultColor: "white"
            activeColor: "deepskyblue"
            source: rootAudio.playbackState == MediaPlayer.PlayingState ? "../img/pause.svg" : "../img/play.svg"
            anchors.centerIn: parent
            onClicked: if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                           rootAudio.pause()
                       } else {
                           rootAudio.play()
                       }
        }

        SvgIcon{
            id: bigPlayerForwardButton
            width: parent.height*0.6
            height: width
            defaultColor: "white"
            activeColor: "deepskyblue"

            source: "../img/forward.svg"

            anchors{
                left: bigPlayerPlayButton.right
                leftMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }

            onClicked:{
                rootAudio.stop()
                currentPlayListModel.currentIndex++
            }
        }

        SvgIcon{
            id: bigPlayerLikeButton
            width: parent.height*0.6
            height: width
            defaultColor: "white"
            activeColor: "deepskyblue"

            source: "../img/heart.svg"

            anchors{
                left: bigPlayerForwardButton.right
                leftMargin: Theme.paddingLarge
                verticalCenter: parent.verticalCenter
            }
            onClicked: user.like("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
        }
    }
}

