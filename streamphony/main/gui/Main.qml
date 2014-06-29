import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
    id: loginWindow
    visible: true
    title: "Streamphony"

    color: "#444444"
    width: 585
    height: 335

    Rectangle {
        x: 87
        y: 8
        color: "#454545"
        anchors.fill: parent

        Loader {
            id: pageLoader
        }

        Text {
            id: text1
            x: 205
            y: 112
            text: qsTr("Login with Facebook")
            font.family: "Times New Roman"
            font.pixelSize: 25
        }

        Button {
            id: buttonRew
            y: 249
            text: "Login"
            anchors.leftMargin: 352
            anchors.left: parent.left
            onClicked: {
                _xmppManager.signIn(userNameInput.text, passwordInput.text, passwordCheckBox.checked);
            }
        }

        Connections {
            target: _xmppManager
            onSignInSuccessfull: {
                loginWindow.close();
                pageLoader.source = "MainWindow.qml"
            }
        }

        Rectangle {
            border.color: "#0b3d0a"
            x: 193
            y: 166
            width: 107
            height: 18

            TextInput {
                id: userNameInput
                text: _settingsManager.xmppUsername;
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.topMargin: 2
                font.pixelSize: 12
            }
        }

        Rectangle {
            border.width: 1
            border.color: "#02cf0e"
            x: 193
            y: 190
            width: 239
            height: 20

            TextInput {
                id: passwordInput
                text: _settingsManager.password;
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 8
                anchors.topMargin: 0
                font.family: "Times New Roman"
                echoMode: TextInput.Password
                font.pixelSize: 12
            }
        }

        Text {
            id: text2
            x: 306
            y: 168
            text: qsTr("@chat.facebook.com")
            font.pixelSize: 12
        }

    }

    CheckBox {
        id: passwordCheckBox
        x: 191
        y: 221
        text: qsTr("Remember password")
        checked: true
    }
}
