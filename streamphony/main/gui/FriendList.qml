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
                Rectangle {
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
                        text: name
                        anchors.verticalCenter: avatarHolder.verticalCenter
                        anchors.left: avatarHolder.right
                        anchors.leftMargin: 10
                        font.bold: true
                        color: "#ccdacc"
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

