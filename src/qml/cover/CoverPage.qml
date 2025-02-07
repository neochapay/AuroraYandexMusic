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
        anchors {
            top: bgCover.bottom
            topMargin: Theme.paddingMedium
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width - Theme.paddingMedium * 2
        height: parent.height - bgCover.height - activecover.height
        clip: true
        spacing: Theme.paddingSmall

        Item {
            width: parent.width
            height: artistLabel.height
            clip: true

            Label {
                id: artistLabel
                width: contentWidth
                anchors.verticalCenter: parent.verticalCenter
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
                text: qsTr("Play")

                property bool needsAnimation: contentWidth > parent.width

                onNeedsAnimationChanged: updatePosition()
                onTextChanged: updatePosition()

                function updatePosition() {
                    if (needsAnimation) {
                        x = 0
                        artistAnimation.restart()
                    } else {
                        x = (parent.width - contentWidth)/2
                        artistAnimation.stop()
                    }
                }

                SequentialAnimation {
                    id: artistAnimation
                    loops: Animation.Infinite
                    running: false

                    NumberAnimation {
                        target: artistLabel
                        property: "x"
                        from: 0
                        to: parent.width - artistLabel.contentWidth
                        duration: Math.max(2000, (artistLabel.contentWidth - parent.width) * 20)
                        easing.type: Easing.InOutSine
                    }
                    PauseAnimation { duration: 800 }
                    NumberAnimation {
                        target: artistLabel
                        property: "x"
                        from: parent.width - artistLabel.contentWidth
                        to: 0
                        duration: Math.max(2000, (artistLabel.contentWidth - parent.width) * 20)
                        easing.type: Easing.InOutSine
                    }
                    PauseAnimation { duration: 800 }
                }
            }
        }

        Item {
            width: parent.width
            height: songLabel.height
            clip: true

            Label {
                id: songLabel
                width: contentWidth
                anchors.verticalCenter: parent.verticalCenter
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
                font.weight: Font.Medium
                text: qsTr("My wave")

                property bool needsAnimation: contentWidth > parent.width

                onNeedsAnimationChanged: updatePosition()
                onTextChanged: updatePosition()

                function updatePosition() {
                    if (needsAnimation) {
                        x = 0
                        songAnimation.restart()
                    } else {
                        x = (parent.width - contentWidth)/2
                        songAnimation.stop()
                    }
                }

                SequentialAnimation {
                    id: songAnimation
                    loops: Animation.Infinite
                    running: false

                    NumberAnimation {
                        target: songLabel
                        property: "x"
                        from: 0
                        to: parent.width - songLabel.contentWidth
                        duration: Math.max(2000, (songLabel.contentWidth - parent.width) * 20)
                        easing.type: Easing.InOutSine
                    }
                    PauseAnimation { duration: 800 }
                    NumberAnimation {
                        target: songLabel
                        property: "x"
                        from: parent.width - songLabel.contentWidth
                        to: 0
                        duration: Math.max(2000, (songLabel.contentWidth - parent.width) * 20)
                        easing.type: Easing.InOutSine
                    }
                    PauseAnimation { duration: 800 }
                }
            }
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
