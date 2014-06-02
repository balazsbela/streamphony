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
        id: searchField
        x: 345
        y: 40
        width: 218
        height: 22
        placeholderText: qsTr("Text Field")
    }

    Button {
        id: searchButton
        x: 569
        y: 40
        text: qsTr("Search")
        onClicked: _connectionManager.searchNodes(searchField.text)
    }

    Label {
        id: label1
        x: 420
        y: 0
        color: "#dddddd"
        text: qsTr("Search for a song")
        font.pointSize: 16
    }

    Results {
        id: results
    }
}

