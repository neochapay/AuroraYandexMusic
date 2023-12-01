import QtQuick 2.0
import Sailfish.Silica 1.0

import ru.neochapay.yandexmusic 1.0

Item {
    id: mainPlayer

    property string title: "Unknow title"
    property string artist: "Unknow artist"
    property string cover

    width: parent.width
    height: Theme.itemSizeLarge
    anchors{
        bottom: parent.bottom
    }
    z: 99

    MouseArea{
        enabled: littlePlayer.visible
        anchors.fill: parent
        onClicked: {
            mainPlayer.height = mainPlayer.parent.height
        }
    }

    Item{
        id: littlePlayer
        width: parent.width
        height: Theme.itemSizeLarge
        visible: littlePlayer.height == mainPlayer.height

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
            source: "https://"+mainPlayer.cover.replace("%%", "50x50")
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
            width: parent.height
            height: width

            anchors.right: parent.right

            icon.source: "image://theme/icon-m-play?" + (pressed
                                                         ? Theme.highlightColor
                                                         : Theme.primaryColor)
            onClicked: console.log("Play clicked!")
        }
    }

    Rectangle{
        id: bigPlayer
        width: parent.width
        height: mainPlayer.parent.height
        color: "green"
        visible: !littlePlayer.visible
    }


    Behavior on height {
        NumberAnimation { duration: 500 }
    }

    onVisibleChanged: {
        if(currentPlayListModel.rowCount > 0) {
            mainPlayer.title = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).title
            mainPlayer.artist = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).artists[0].name
            mainPlayer.cover = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).albums[0].coverUri
            musicFetcher.load(currentPlayListModel.getTrack(currentPlayListModel.currentIndex))
        }
    }
}
