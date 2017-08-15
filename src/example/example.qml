import QtQuick 2.4
import QtMultimedia 5.0
import "video.js" as Ctrl

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "blue"

    Component.onCompleted: Ctrl.load();

    MouseArea {
        anchors.fill: parent
        onClicked: Ctrl.click();
    }
}
