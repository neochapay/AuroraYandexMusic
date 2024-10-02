import QtQuick 2.0
import Sailfish.Silica 1.0

import ru.neochapay.ourmusic 1.0

import QtMultimedia 5.5

Item {
    id: myWave
    width: parent.width
    height: width/2

    Label{
        id: myWaveLabel
        anchors{
            horizontalCenter: parent.horizontalCenter
            bottom: myWavePlayButton.top
            bottomMargin: Theme.paddingLarge
        }
    }

    SvgIcon {
        id: myWavePlayButton

        width: parent.height/3
        height: width
        anchors.centerIn: parent

        source: "../img/play.svg"

        onClicked: {
            ourMusic.isMyWave = true
            rotor.getStationTracks();
        }
    }

    Connections{
        target: rootAudio
        onStatusChanged: {
            if(rootAudio.status == MediaPlayer.NoMedia) {
                myWavePlayButton.source = "../img/ban.svg"
            }

            if(rootAudio.status == MediaPlayer.Loaded) {
                myWavePlayButton.source = "../img/play.svg"
            }
        }

        onPlaybackStateChanged: {
            if(rootAudio.playbackState == MediaPlayer.PlayingState) {
                myWavePlayButton.source = "../img/pause.svg"
            } else {
                myWavePlayButton.source = "../img/play.svg"
            }
        }
    }
}
