import QtQuick 2.4
import QtMultimedia 5.0

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "blue"

    Video {
        width: 1280
        height: 720
        anchors.centerIn: parent
        source: "file:///usr/share/movies/big_buck_bunny_720p_surround.avi"
        autoPlay: true
    }
}
