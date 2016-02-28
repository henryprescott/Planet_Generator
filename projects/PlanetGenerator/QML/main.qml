import QtQuick 2.0
import OpenGLUnderQML 1.0

Item {
    id: page
    width: 320; height: 480

    ViewPort {
        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }
}