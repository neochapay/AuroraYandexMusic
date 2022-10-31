import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

import "../components"

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: mainView
        anchors.fill: parent

        BusyIndicator {
            id: busyIndicator
            running: visible
            visible: true
            anchors.centerIn: parent
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Search")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("SearchPage.qml"))
            }
            MenuItem {
                text: qsTr("Logout")
                onClicked: {
                    auth.removeAccessToken()
                    auth.removeUserId()
                    pageContainer.replace(Qt.resolvedUrl("LoginPage.qml"))
                }
            }
            MenuItem {
                text: qsTr("View playlist")
                onClicked: pageStack.animatorPush(Qt.resolvedUrl("PlaylistPage.qml"))
            }
        }

        PageHeader {
            id: header
            title: qsTr("My wave")
        }

        contentHeight: mainPage.height
        Row {
            id: searchrow
            height: childrenRect.height
            width: parent.width
            visible: false

        }

        Column {
            id: column
            anchors{
                top: header.bottom
                topMargin: Theme.paddingLarge
                left: parent.left
                leftMargin: Theme.paddingLarge
            }
            width: parent.width-Theme.paddingLarge*2
            height: parent.height-header.height-Theme.paddingLarge*2

            visible: !busyIndicator.visible
            spacing: Theme.paddingLarge


            Image{
                id: coverImage
                width: column.width
                height: width
                fillMode: Image.PreserveAspectFit
                source: "/usr/share/icons/hicolor/172x172/apps/org.ilyavysotsky.yasailmusic.png"
            }

            Label{
                id: songTitle
                color: Theme.primaryColor
                font.bold: true
                font.pixelSize: Theme.fontSizeLarge
            }

            Label{
                id: artistTitle
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium
            }


            Item{
                id: downloadAndProgressItem
                height: Theme.fontSizeLarge
                width: parent.width

                Rectangle{
                    id: downloadProgressItem
                    width: 0
                    height: Theme.dp(1)
                    color: Theme.highlightColor
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }
                }

                Rectangle{
                    id: playingProgressItem
                    width: 0
                    height: Theme.dp(3)
                    color: Theme.primaryColor
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }
                }

                MouseArea{
                    id: seekArea
                    anchors.fill: parent
                    onClicked: {
                        if(downloadProgressItem.width == downloadAndProgressItem.width) {
                            var current_poz = (mouseX-x)/width
                            rootAudio.seek(rootAudio.duration*current_poz)
                        }
                    }
                }
            }

            Item {
                id: controlsArea
                width: parent.width
                height: childrenRect.height

                MediaButton {
                    id: prevButton
                    anchors{
                        right: playButton.left
                        rightMargin: Theme.paddingLarge
                        verticalCenter: playButton.verticalCenter
                    }
                    visible: playListModel.currentIndex > 0

                    source: "image://theme/icon-cover-previous-song"
                    mouseArea.onClicked: {
                        --playListModel.currentIndex
                    }
                }

                MediaButton {
                    id: playButton
                    anchors.centerIn: parent
                    source: rootAudio.playbackState === MediaPlayer.PlayingState ?
                                "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
                    mouseArea.onClicked: {
                        if(playListModel.currentIndex === -1) {
                            playListModel.currentIndex = 0;
                        }

                        if (rootAudio.playbackState == MediaPlayer.PlayingState) {
                            rootAudio.pause()
                        } else {
                            rootAudio.play()
                        }
                    }
                }

                MediaButton {
                    id: nextButton
                    anchors{
                        left: playButton.right
                        leftMargin: Theme.paddingLarge
                        verticalCenter: playButton.verticalCenter
                    }
                    visible: !fileCacher.downloading

                    source: "image://theme/icon-cover-next-song"
                    mouseArea.onClicked: {
                        ++playListModel.currentIndex
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        playListModel.loadMyWave();
    }

    Connections{
        target: playListModel
        onRowCountChanged: {
            if(playListModel.rowCount > 0) {
                busyIndicator.visible = false
                if(playListModel.currentIndex == -1) {
                    var cover = "https://"+playListModel.get(0).albumCover;
                    coverImage.source = cover.slice(0,-2)+"1000x1000"
                    songTitle.text = playListModel.get(0).trackName
                    artistTitle.text = playListModel.get(0).artistName
                }
            }
        }

        onCurrentIndexChanged: {
        }
    }

    Connections{
        target: fileCacher
        onDownloadProgress: {
            downloadProgressItem.width = downloadAndProgressItem.width*progress
        }
        onFileSaved: {
            var cover = "https://"+playListModel.get(playListModel.currentIndex).albumCover;
            coverImage.source = cover.slice(0,-2)+"1000x1000"
            songTitle.text = playListModel.get(playListModel.currentIndex).trackName
            artistTitle.text = playListModel.get(playListModel.currentIndex).artistName
        }
    }

    Connections{
        target: rootAudio
        onPositionChanged: {
            playingProgressItem.width = downloadAndProgressItem.width*(rootAudio.position/rootAudio.duration)
        }
    }
}
