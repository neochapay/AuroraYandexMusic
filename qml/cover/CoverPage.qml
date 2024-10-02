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
        width: parent.width - Theme.paddingMedium * 2
        anchors{
            top: parent.top
	    horizontalCenter: parent.horizontalCenter
	    margins: Theme.paddingMedium
        }

        fillMode: Image.PreserveAspectFit
        source: "/usr/share/icons/hicolor/172x172/apps/ru.neochapay.ourmusic.png"
    }

    Column {
        id: titleColumn
        anchors{
            top: bgCover.bottom
            topMargin: Theme.paddingMedium
            horizontalCenter: parent.horizontalCenter
            rightMargin: Theme.paddingMedium
            leftMargin: Theme.paddingMedium
        }

        width: parent.width - Theme.paddingMedium * 2
        height: parent.height - bgCover.height - activecover.height
        clip: true
        spacing: paddingSmall

        Label {
            id: artistLabel
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeTiny
            truncationMode: TruncationMode.Fade
            text: qsTr("Play")
        }

        Label {
            id: songLabel
            width: contentWidth>cover.width ? cover.width : contentWidth
            anchors.horizontalCenter: parent.horizontalCenter
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeExtraSmall
            font.weight: Font.Medium
            truncationMode: TruncationMode.Fade
            text: qsTr("My wave")
        }
    }

    CoverActionList {
        id: activecover
        CoverAction {
            iconSource: "image://theme/icon-m-like"
            onTriggered: user.like("track", currentPlayListModel.getTrack(currentPlayListModel.currentIndex).trackId)
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
            onTriggered: {
                rootAudio.stop()
                currentPlayListModel.currentIndex++
            }
        }
    }
}
