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
                bigPlayerTrackCover.source = "https://"+track.albums[0].coverUri.replace("%%", "400x400")
                bigPlayerInfoItemArtistCover.source = "https://"+track.artists[0].coverUri.replace("%%", "100x100")
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
                //onClicked: pageStack.push(Qt.resolvedUrl("pages/ArtistPage.qml", { artistId : track.artists[0].artistId }))
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
    }
}

