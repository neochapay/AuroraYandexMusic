import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView
        model: playListModel
        anchors.fill: parent
        header: PageHeader {
            title: qsTr("Current tracks")
        }
        delegate: BackgroundItem {
            id: delegate

            Image {
                id: cover
                source: "https://"+albumCover.replace("%%","200x200")
                width: height
                height: parent.height
            }

            Label {
                x: Theme.horizontalPageMargin
                text: artistName + " - " + trackName
                anchors{
                    left: cover.right
                    leftMargin: 20
                    top: parent.top
                }

                color:  playListModel.currentIndex === index ? Theme.highlightColor : Theme.primaryColor
            }

            onClicked: {
                playListModel.setCurrentIndex(index)
            }
        }
        VerticalScrollDecorator {}
    }
}
