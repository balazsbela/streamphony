import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

Rectangle {
    id: buttonPanel

    signal nextPressed
    signal previousPressed
    signal playPausePressed

    function changePlayPauseIcon(currentIcon){
        if(currentIcon == "play")
            buttonPlayPause.iconSource = "icons/play.png"
        else
            buttonPlayPause.iconSource = "icons/pause.png"
    }

    color : parent.color
    width: 100; height: 40

    Item {
        id: buttons
        anchors { top: parent.top; topMargin: -4 }

        ToolButton {
            id: buttonRew
            iconSource: "icons/rew.png"
            anchors.left: parent.left
            onClicked: buttonPanel.previousPressed()
        }

        ToolButton {
            id: buttonPlayPause
            iconSource: "icons/play.png"
            anchors.left: buttonRew.right
            onClicked: buttonPanel.playPausePressed()
        }

        ToolButton {
            id: buttonFfw
            iconSource: "icons/ffw.png"
            anchors.left: buttonPlayPause.right
            onClicked: buttonPanel.nextPressed()
        }

    }
}
