import QtQuick 2.0
import Sailfish.Silica 1.0

import org.ilyavysotsky.yasailmusic 1.0

Item {
    id: mainPlayer

    property string title: "Unknow title"
    property string artist: "Unknow artist"

    width: parent.width
    height: Theme.itemSizeLarge
    anchors{
        bottom: parent.bottom
    }
    z: 99

    Item{
        id: littlePlayer
        width: parent.width
        height: Theme.itemSizeLarge
        visible: littlePlayer.height == mainPlayer.height

        IconButton {
            id: littlePlayerLikeButton
            width: parent.height
            height: width

            anchors{
                left: parent.left
            }

            icon.source: "image://theme/icon-m-like?" + (pressed
                                                         ? Theme.highlightColor
                                                         : Theme.primaryColor)
            onClicked: console.log("Like clicked!")
        }


        Label{
            id: littlePlayerTitleLabel
            anchors{
                left: littlePlayerLikeButton.right
                bottom: parent.verticalCenter
            }
            width: parent.width - littlePlayerLikeButton.width - littlePlayerPlayButton.width
            height: parent.height / 2
            text: mainPlayer.title
            color: Theme.highlightColor
            verticalAlignment: Text.AlignVCenter
        }

        Label{
            id: littlePlayerArtistLabel
            anchors{
                left: littlePlayerLikeButton.right
                top: parent.verticalCenter
            }
            width: parent.width - littlePlayerLikeButton.width - littlePlayerPlayButton.width
            height: parent.height / 2
            text: mainPlayer.artist
            color: Theme.primaryColor
            verticalAlignment: Text.AlignTop
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

    MouseArea{
        enabled: littlePlayer.visible
        anchors.fill: parent
        onClicked: {
            mainPlayer.height = mainPlayer.parent.height
        }
    }

    Behavior on height {
        NumberAnimation { duration: 500 }
    }

    onVisibleChanged: {
        if(currentPlayListModel.rowCount > 0) {
            mainPlayer.title = currentPlayListModel.getTrack(0).title
            mainPlayer.artist = currentPlayListModel.getTrack(0).artists[0].name
        }
    }
}
