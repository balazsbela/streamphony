import QtQuick 2.0

import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

Rectangle {
    id: friendsListComponent;

    ListView {
        id: friendList
        clip: true
        width: 339
        height: 490
        model: _rosterModel
        delegate: Item {
            x: 5
            width: parent.width
            height: 45
            Row {
                id: row1
                width : parent.width
                height : parent.height

                Rectangle {
                    id: mainContainer
                    width: parent.width
                    height: parent.height
                    color: friendsListComponent.color

                    Rectangle {
                        id: avatarHolder
                        x:5
                        y:5
                        color:parent.color
                        border.color: "#004400"
                        border.width: 1
                        width: 34
                        height: 34

                        Image {
                            id: avatar
                            smooth: true
                            cache:false;
                            x:1
                            y:1
                            source: "image://avatars/" + bareJid
                            function reload(bareJid) {
                                 var oldSource = source;
                                 source = "";
                                 source = oldSource;
                            }
                        }
                    }

                    Connections {
                        target: _xmppManager
                        onAvatarChanged: avatar.reload(bareJid);
                    }

                    Text {
                        id: nameText
                        text: name
                        anchors.verticalCenter: avatarHolder.verticalCenter
                        anchors.left: avatarHolder.right
                        anchors.leftMargin: 10
                        font.bold: true
                        color: "#ccdacc"
                    }

                    Text {
                        text: _connectionManager.isDiscovered(bareJid) ? "Discovered" :  "Discovering";
                        anchors.verticalCenter: avatarHolder.verticalCenter
                        anchors.left: avatarHolder.right
                        anchors.leftMargin: 190
                        font.bold: true
                        color: _connectionManager.isDiscovered(bareJid) ? "#62E327" : "#F4CF3D"
                    }

                    MouseArea {
                        anchors.fill : parent
                        onClicked : {
                            if (_connectionManager.isDiscovered(bareJid)) {
                                 _connectionManager.listNode(bareJid);
                            }
                        }
                    }
                }
                spacing: 1
            }
        }
    }

    ScrollBar {
         flickable: friendList
    }
}

