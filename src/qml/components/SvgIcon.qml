import QtQuick 2.0
import Sailfish.Silica 1.0

Image {
    id: clickIcon

    signal clicked
    property string activeColor: Theme.highlightColor
    property string defaultColor: Theme.primaryColor

    opacity: 0.6

    fillMode: Image.PreserveAspectFit

    sourceSize.width: width
    sourceSize.height: height

    /*layer.effect: ShaderEffect {
        id: shaderItem
        property color color: clickMouseArea.pressed ?  clickIcon.activeColor : clickIcon.defaultColor

        fragmentShader: "
            varying mediump vec2 qt_TexCoord0;
            uniform highp float qt_Opacity;
            uniform lowp sampler2D source;
            uniform highp vec4 color;
            void main() {
                highp vec4 pixelColor = texture2D(source, qt_TexCoord0);
                gl_FragColor = vec4(mix(pixelColor.rgb/max(pixelColor.a, 0.00390625), color.rgb/max(color.a, 0.00390625), color.a) * pixelColor.a, pixelColor.a) * qt_Opacity;
            }
        "
    }
    layer.enabled: true
    layer.samplerName: "source"*/

    MouseArea{
        id: clickMouseArea
        anchors.fill: parent
        enabled: clickIcon.enabled
        onClicked: {
            clickIcon.clicked()
        }
    }
}
