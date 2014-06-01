import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
    id: applicationWindow1
    visible: true
    title: "Streamphony"

    color: "#444444"
    width: 800
    height: 600
    minimumHeight: 600
    minimumWidth: 800

    PlaybackScreen {
        id: playbackScreen
        anchors.top: parent.top
        anchors.topMargin: 0
    }

    FriendList {
        id: friendList
        anchors.top : playbackScreen.bottom;
        color: "#333333"
    }

    TextField {
        id: textField1
        x: 345
        y: 40
        width: 218
        height: 22
        placeholderText: qsTr("Text Field")
    }

    Button {
        id: button1
        x: 569
        y: 40
        text: qsTr("Search")
    }

    Label {
        id: label1
        x: 420
        y: 0
        color: "#dddddd"
        text: qsTr("Search for a song")
        font.pointSize: 16
    }

    ListView {
        id: listView1
        x: 345
        y: 102
        width: 304
        height: 286
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        delegate: Item {
            x: 5
            width: 80
            height: 40
            Row {
                id: row1
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                }

                Text {
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                }
                spacing: 10
            }
        }
    }
}

