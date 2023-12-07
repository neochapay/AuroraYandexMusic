import QtQuick 2.0
import Sailfish.Silica 1.0

import QtMultimedia 5.5

Item {
    id: myWave
    width: parent.width
    height: width/2

    Label{
        id: myWaveLabel
        anchors.centerIn: parent
        text: qsTr("My wave")
    }

    IconButton {
        id: myWavePlayButton

        property string iconName: "icon-m-play"

        width: myWaveLabel.height
        height: width
        anchors{
            top: myWaveLabel.bottom
            topMargin: Theme.paddingLarge
            horizontalCenter: myWave.horizontalCenter
        }

        icon.source: "image://theme/"+iconName+"?" + (pressed
                                                      ? Theme.highlightColor
                                                      : Theme.primaryColor)
        onClicked: {
            if(currentPlayListModel.rowCount > 0 && currentPlayListModel.currentIndex == -1) {
                currentPlayListModel.currentIndex = 0
            }

            if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                rootAudio.pause()
            } else {
                rootAudio.play()
            }
        }
    }

    Connections{
        target: rootAudio
        onStatusChanged: {
            if(rootAudio.status == MediaPlayer.NoMedia) {
                myWavePlayButton.iconName = "icon-m-cancel"
            }

            if(rootAudio.status == MediaPlayer.Loaded) {
                myWavePlayButton.iconName = "icon-m-play"
            }
        }

        onPlaybackStateChanged: {
            if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                myWavePlayButton.iconName = "icon-m-pause"
            } else {
                myWavePlayButton.iconName = "icon-m-play"
            }
        }
    }
}
