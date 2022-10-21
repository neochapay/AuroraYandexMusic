import QtQuick 2.1
import Sailfish.Silica 1.0

Image {
    property alias mouseArea: mbMouseArea
    property alias buttonEnabled: mbRect.enabled

    width: Theme.iconSizeLarge
    height: Theme.iconSizeLarge
    MouseArea {
        id: mbMouseArea
        anchors.fill: parent
    }
    Rectangle {
        id: mbRect
        anchors.fill: parent
        anchors.margins: Theme.paddingSmall
        color: mbMouseArea.pressed
               ? Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
               : Theme.rgba(Theme.primaryColor, 0.2)
        radius: width / 2.0
    }
}
