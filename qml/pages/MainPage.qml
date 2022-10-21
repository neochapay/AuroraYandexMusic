import QtQuick 2.0
import Sailfish.Silica 1.0

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
        Row {
            id: column
            height: childrenRect.height
            width: childrenRect.width

            anchors.centerIn: parent
            visible: !busyIndicator.visible

            MediaButton {
                id: medbut
                source: rootAudio.isPlaying ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
                mouseArea.onClicked: {

                    if(playListModel.currentIndex === -1) {
                        playListModel.currentIndex = 0;
                    }

                    if (rootAudio.isPlaying) {
                        rootAudio.pause()
                        source = "image://theme/icon-cover-pause"
                    } else {
                        rootAudio.play()
                        source = "image://theme/icon-cover-play"
                    }
                }
            }

            Label {
                id: mediaButtLabel
                text: rootAudio.isPlaying ? qsTr("Pause") : qsTr("Play")
                font.pixelSize: Theme.fontSizeMedium

                anchors{
                    verticalCenter: medbut.verticalCenter
                }
            }
        }
    }

    Component.onCompleted: {
        playListModel.loadMyWave();
    }

    Connections{
        target: playListModel
        onLoadFirstDataFinished: busyIndicator.visible = false
    }
}
