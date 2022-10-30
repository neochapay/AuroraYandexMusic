import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

CoverBackground {

    Image{
        id: bgCover
        width: parent.width
        height: width
        anchors{
            top: parent.top
        }

        fillMode: Image.PreserveAspectFit
        source: "/usr/share/icons/hicolor/172x172/apps/org.ilyavysotsky.yasailmusic.png"
    }

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingMedium
        Label {
            visible: rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState
            id: artist
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            text: (rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState) ? playListModel.get(playListModel.currentIndex).artistName : "Idle"
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            truncationMode: TruncationMode.Fade
        }

        Label {
            visible: rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState
            id: song
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            text: (rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState) ? playListModel.get(playListModel.currentIndex).trackName : "Idle"
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            truncationMode: TruncationMode.Fade
        }
    }

    CoverActionList {
        id: activecover
        enabled: rootAudio.playbackState === MediaPlayer.PlayingState || rootAudio.playbackState === MediaPlayer.PausedState
        CoverAction {
            iconSource: "image://theme/icon-cover-previous-song"
            onTriggered: {
                --playListModel.currentIndex
            }
        }

        CoverAction {
            iconSource: (rootAudio.playbackState === MediaPlayer.PlayingState) ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: {
                if (rootAudio.playbackState === MediaPlayer.PlayingState) {
                    rootAudio.pause()
                } else {
                    rootAudio.play()
                }
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: {
                ++playListModel.currentIndex
            }
        }
    }

    Connections{
        target: playListModel
        onCurrentIndexChanged: {
            song.text = playListModel.get(currentIndex).trackName
            artist.text = playListModel.get(currentIndex).artistName
            var cover = "https://"+playListModel.get(currentIndex).albumCover;
            //cover string always end on %25%25 in must to replace to 100x100
            //or other multiple of one hundred
            bgCover.source = cover.slice(0,-2)+"100x100"
        }
    }
}
