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

    onCoverChanged: {
        if(mainPlayer.cover.length != 0) {
            littlePlayer.cover = "https://"+mainPlayer.cover.replace("%%", "50x50")
        }
    }

    MouseArea{
        enabled: littlePlayer.visible
        anchors.fill: parent
        onClicked: {
            mainPlayer.height = mainPlayer.parent.height
        }
    }


    LittlePlayer{
        id: littlePlayer
        width: parent.width
        height: Theme.itemSizeLarge
        visible: littlePlayer.height == mainPlayer.height
    }

    Rectangle{
        id: bigPlayer
        width: parent.width
        height: mainPlayer.parent.height
        color: "black"
        visible: !littlePlayer.visible

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
    }


    Behavior on height {
        NumberAnimation { duration: 500 }
    }

    onVisibleChanged: {
        loadCurrentData()
    }

    function loadCurrentData() {
        if(currentPlayListModel.rowCount > 0) {
            mainPlayer.title = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).title
            mainPlayer.artist = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).artists[0].name
            mainPlayer.cover = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).albums[0].coverUri
            musicFetcher.load(currentPlayListModel.getTrack(currentPlayListModel.currentIndex))

            littlePlayer.bgColor = currentPlayListModel.getTrack(currentPlayListModel.currentIndex).derivedColors.miniPlayer
        }
    }
}
