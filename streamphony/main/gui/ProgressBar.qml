import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Rectangle {
    id: progressBar

    property int total: 0
    property alias barWidth: bar.width

    signal seek(variant current)

    function update(elapsed){
        console.log(elapsed + "/" + total);
        bar.width = (elapsed/total) * width
    }

    color: "#000000"
    radius: 10
    smooth: true

    Rectangle {
        id: bar
        width: 0; height: parent.height
        color: "#0AC419"
        anchors { left: parent.left; top: parent.top }
        radius: 10
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: progressBar.seek((mouse.x/progressBar.width)*progressBar.total)
    }
}
