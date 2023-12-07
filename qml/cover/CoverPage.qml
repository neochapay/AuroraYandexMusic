import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

CoverBackground {
    id: cover

    Connections{
        target: currentPlayListModel
        onCurrentIndexChanged: {
            var track = currentPlayListModel.getCurrentTrack();
            if(track == null) {
                return
            }
            songLabel.text = track.title
            artistLabel.text = track.artists[0].name
            bgCover.source = "https://"+track.albums[0].coverUri.replace("%%", "200x200")
        }
    }

    Image{
        id: bgCover
        width: parent.width * 0.8
        height: width
        anchors{
            top: parent.top
            topMargin: parent.width*0.1
            left: parent.left
            leftMargin: parent.width*0.1
        }

        fillMode: Image.PreserveAspectFit
        source: "/usr/share/icons/hicolor/172x172/apps/ru.neochapay.yandexmusic.png"
    }

    Column {
        id: titleColumn
        anchors{
            top: bgCover.bottom
            topMargin: parent.width*0.1
            left: parent.left
            leftMargin: parent.width*0.1
        }

        width: parent.width * 0.8
        spacing: parent.width*0.1

        Label {
            id: artistLabel
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            truncationMode: TruncationMode.Fade
            text: qsTr("Play")
        }

        Label {
            id: songLabel
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            truncationMode: TruncationMode.Fade
            text: qsTr("My wave")
        }
    }

    CoverActionList {
        id: activecover
        CoverAction {
            iconSource: "image://theme/icon-m-like"
            onTriggered: console.log("like pressed")
        }

        CoverAction {
            iconSource: (rootAudio.playbackState === MediaPlayer.PlayingState) ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: {
                if(currentPlayListModel.rowCount > 0 && currentPlayListModel.currentIndex == -1) {
                    currentPlayListModel.currentIndex = 0
                }

                if (rootAudio.playbackState === MediaPlayer.PlayingState) {
                    rootAudio.pause()
                } else {
                    rootAudio.play()
                }
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: console.log("next pressed")
        }
    }
}
