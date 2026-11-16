import QtQuick 2.0
import Sailfish.Silica 1.0

import ru.neochapay.ourmusic 1.0

Item {
    id: mainPlayer

    property var stationDetector: null
    property var stationController: null
    property bool playOnStation: false

    width: parent.width
    height: Theme.itemSizeLarge
    anchors{
        bottom: parent.bottom
        bottomMargin: 0
    }
    z: 99

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
        stationController: mainPlayer.stationController
        playOnStation: mainPlayer.playOnStation
    }

    BigPlayer{
        id: bigPlayer
        width: parent.width
        height: mainPlayer.parent.height
        visible: !littlePlayer.visible
        stationDetector: mainPlayer.stationDetector
        stationController: mainPlayer.stationController
        playOnStation: mainPlayer.playOnStation
        onPlayOnStationChanged: mainPlayer.playOnStation = playOnStation
        anchors{
            bottom: parent.bottom
        }
    }

    Behavior on height {
        NumberAnimation { duration: 300 }
    }
}
