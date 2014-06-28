import QtQuick 2.0

Rectangle {
    id: resultListComponent

    function next() {
        if (resultList.currentIndex < resultList.count - 1) {
            resultList.currentIndex++;
            if (resultList.currentItem)
                _connectionManager.play(resultList.currentItem.currentSong,
                                        resultList.currentItem.currentJid)
        }
    }

    function previous() {
        if (resultList.currentIndex > 0) {
            resultList.currentIndex--;
            if (resultList.currentItem)
                _connectionManager.play(resultList.currentItem.currentSong,
                                        resultList.currentItem.currentJid)
        }
    }

    ListView {
        id: resultList
        clip: true

        width:500
        height:500

        x: 345
        y: 102

        highlight: Rectangle {
            id: selectionRectangle
            color: "#99F22C"
            smooth: true
            opacity:0.6
        }

        model: _searchResultModel
        delegate: Item {

            property string currentSong : filename
            property string currentJid : jid

            x: 5

            width:500
            height:50

            Row {
                width : parent.width
                height : parent.height

                Rectangle {
                    id: row1
                    color: "#333333"
                    width : parent.width
                    height : parent.height
                    clip:true

                    Text {
                        text: filename + " - " +  _xmppManager.fullName(jid)
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                        color: "#ccdacc"
                        clip: true
                        width : parent.width
                    }

                    MouseArea {
                        anchors.fill : parent
                        onClicked : {
                            resultList.currentIndex = index;
                            _connectionManager.play(filename, jid)
                        }
                    }
                }
                spacing: 10
            }
        }
    }

    ScrollBar {
        flickable: resultList
    }
}
