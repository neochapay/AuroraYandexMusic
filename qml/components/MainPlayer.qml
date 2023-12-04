import QtQuick 2.0
import Sailfish.Silica 1.0

import ru.neochapay.yandexmusic 1.0

Item {
    id: mainPlayer

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


    LittlePlayer{
        id: littlePlayer
        width: parent.width
        height: Theme.itemSizeLarge
        visible: littlePlayer.height == mainPlayer.height
    }

    BigPlayer{
        id: bigPlayer
        width: parent.width
        height: mainPlayer.parent.height
        visible: !littlePlayer.visible
    }

    Behavior on height {
        NumberAnimation { duration: 300 }
    }
}
