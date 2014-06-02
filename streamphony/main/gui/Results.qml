import QtQuick 2.0

ListView {
    id: listView1
    x: 345
    y: 102
    width: 304
    height: 286
    model: _searchResultModel
    delegate: Item {
        x: 5
        width: 80
        height: 40
        Row {
            id: row1
            Text {
                text: filename
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
            }

            Text {
                text: jid
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
            }
            spacing: 10
        }
    }
}
