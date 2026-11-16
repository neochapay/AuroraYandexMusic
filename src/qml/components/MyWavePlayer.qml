import QtQuick 2.0
import Sailfish.Silica 1.0

import ru.neochapay.ourmusic 1.0

import QtMultimedia 5.6

Item {
    id: myWave
    width: parent.width
    height: width/2

    property url backgroundVideoSource: ""
    property bool backgroundVideoMuted: true

    MediaPlayer {
        id: backgroundVideoPlayer
        source: myWave.backgroundVideoSource
        autoLoad: true
        loops: MediaPlayer.Infinite
        muted: myWave.backgroundVideoMuted

        onSourceChanged: {
            if (source != "") {
                play()
            } else {
                stop()
            }
        }

        onStatusChanged: {
            if (status === MediaPlayer.Loaded || status === MediaPlayer.Buffered) {
                if (source != "" && playbackState !== MediaPlayer.PlayingState) {
                    play()
                }
            }
        }
    }

    VideoOutput {
        id: backgroundVideoOutput
        anchors.fill: parent
        source: backgroundVideoPlayer
        fillMode: VideoOutput.PreserveAspectCrop
        visible: myWave.backgroundVideoSource != ""
        z: 0
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: myWave.backgroundVideoSource != "" ? 0.25 : 0.0
        visible: myWave.backgroundVideoSource != ""
        z: 1
    }

    Label{
        id: myWaveLabel
        anchors{
            horizontalCenter: parent.horizontalCenter
            bottom: myWavePlayButton.top
            bottomMargin: Theme.paddingLarge
        }
        z: 2
    }

    SvgIcon {
        id: myWavePlayButton

        width: parent.height/3
        height: width
        anchors.centerIn: parent
        opacity: 1.0
        defaultColor: Theme.highlightColor
        activeColor: Theme.primaryColor
        z: 2

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
