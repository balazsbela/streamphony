import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
    visible: true
    title: "Component Gallery"

    width: 640
    height: 420
    minimumHeight: 400
    minimumWidth: 600

    property string loremIpsum:
            "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor "+
            "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor "+
            "incididunt ut labore et dolore magna aliqua.\n Ut enim ad minim veniam, quis nostrud "+
            "exercitation ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. ";

    FileDialog {
        id: fileDialog
        nameFilters: [ "Image files (*.png *.jpg)" ]
        onAccepted: imageViewer.open(fileUrl)
    }

    Action {
        id: openAction
        text: "&Open"
        shortcut: StandardKey.Open
        iconSource: "images/document-open.png"
        onTriggered: fileDialog.open()
        tooltip: "Open an image"
    }

    Action {
        id: copyAction
        text: "&Copy"
        shortcut: StandardKey.Copy
        iconName: "edit-copy"
        enabled: (!!activeFocusItem && !!activeFocusItem["copy"])
        onTriggered: activeFocusItem.copy()
    }

    Action {
        id: cutAction
        text: "Cu&t"
        shortcut: StandardKey.Cut
        iconName: "edit-cut"
        enabled: (!!activeFocusItem && !!activeFocusItem["cut"])
        onTriggered: activeFocusItem.cut()
    }

    Action {
        id: pasteAction
        text: "&Paste"
        shortcut: StandardKey.Paste
        iconName: "edit-paste"
        enabled: (!!activeFocusItem && !!activeFocusItem["paste"])
        onTriggered: activeFocusItem.paste()
    }

    Action {
        id: aboutAction
        text: "About"
        onTriggered: aboutDialog.open()
    }

    ExclusiveGroup {
        id: textFormatGroup

        Action {
            id: a1
            text: "Align &Left"
            checkable: true
            Component.onCompleted: checked = true
        }

        Action {
            id: a2
            text: "&Center"
            checkable: true
        }

        Action {
            id: a3
            text: "Align &Right"
            checkable: true
        }
    }

    Menu {
        id: editmenu
        MenuItem { action: cutAction }
        MenuItem { action: copyAction }
        MenuItem { action: pasteAction }
        MenuSeparator {}
        Menu {
            title: "Text &Format"
            MenuItem { action: a1 }
            MenuItem { action: a2 }
            MenuItem { action: a3 }
            MenuSeparator { }
            MenuItem { text: "Allow &Hyphenation"; checkable: true }
        }
        Menu {
            title: "Font &Style"
            MenuItem { text: "&Bold"; checkable: true }
            MenuItem { text: "&Italic"; checkable: true }
            MenuItem { text: "&Underline"; checkable: true }
        }
    }

    toolBar: ToolBar {
        id: toolbar
        RowLayout {
            id: toolbarLayout
            spacing: 0
            width: parent.width
            ToolButton {
                iconSource: "images/window-new.png"
                onClicked: window1.visible = !window1.visible
                Accessible.name: "New window"
                tooltip: "Toggle visibility of the second window"
            }
            ToolButton { action: openAction }
            ToolButton {
                Accessible.name: "Save as"
                iconSource: "images/document-save-as.png"
                tooltip: "(Pretend to) Save as..."
            }
            Item { Layout.fillWidth: true }
            CheckBox {
                id: enabledCheck
                text: "Enabled"
                checked: true
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem { action: openAction }
            MenuItem {
                text: "Close"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: cutAction }
            MenuItem { action: copyAction }
            MenuItem { action: pasteAction }
            MenuSeparator { }
            MenuItem {
                text: "Do Nothing"
                shortcut: "Ctrl+E,Shift+Ctrl+X"
                enabled: false
            }
            MenuItem {
                text: "Not Even There"
                shortcut: "Ctrl+E,Shift+Ctrl+Y"
                visible: false
            }
            Menu {
                title: "Me Neither"
                visible: false
            }
        }
        Menu {
            title: "&Help"
            MenuItem { action: aboutAction }
        }
    }


    SystemPalette {id: syspal}
    color: syspal.window
    ListModel {
        id: choices
        ListElement { text: "Banana" }
        ListElement { text: "Orange" }
        ListElement { text: "Apple" }
        ListElement { text: "Coconut" }
    }

    TabView {
        id:frame
        enabled: enabledCheck.checked
        tabPosition: controlPage.item ? controlPage.item.tabPosition : Qt.TopEdge
        anchors.fill: parent
        anchors.margins: Qt.platform.os === "osx" ? 12 : 2

        Tab {
            id: controlPage
            title: "Controls"

            Item {
                id: flickable
                anchors.fill: parent
                enabled: enabledCheck.checked

                property int tabPosition: tabPositionGroup.current === r2 ? Qt.BottomEdge : Qt.TopEdge

                RowLayout {
                    id: contentRow
                    anchors.fill:parent
                    anchors.margins: 8
                    spacing: 16
                    ColumnLayout {
                        id: firstColumn
                        Layout.minimumWidth: implicitWidth
                        Layout.fillWidth: false
                        RowLayout {
                            id: buttonrow
                            Button {
                                id: button1
                                text: "Button 1"
                                tooltip:"This is an interesting tool tip"
                                Layout.fillWidth: true
                            }
                            Button {
                                id:button2
                                text:"Button 2"
                                Layout.fillWidth: true
                                menu: Menu {
                                    MenuItem { text: "This Button" }
                                    MenuItem { text: "Happens To Have" }
                                    MenuItem { text: "A Menu Assigned" }
                                }
                            }
                        }
                        ComboBox {
                            id: combo
                            model: choices
                            currentIndex: 2
                            Layout.fillWidth: true
                        }
                        ComboBox {
                            model: Qt.fontFamilies()
                            Layout.fillWidth: true
                            currentIndex: 47
                        }
                        ComboBox {
                            id: editableCombo
                            editable: true
                            model: choices
                            Layout.fillWidth: true
                            currentIndex: 2
                            onAccepted: {
                                if (editableCombo.find(currentText) === -1) {
                                    choices.append({text: editText})
                                    currentIndex = editableCombo.find(editText)
                                }
                            }
                        }
                        RowLayout {
                            SpinBox {
                                id: t1
                                Layout.fillWidth: true
                                minimumValue: -50
                                value: -20
                            }
                            SpinBox {
                                id: t2
                                Layout.fillWidth: true
                            }
                        }
                        TextField {
                            id: t3
                            placeholderText: "This is a placeholder for a TextField"
                            Layout.fillWidth: true
                        }
                        ProgressBar {
                            // normalize value [0.0 .. 1.0]
                            value: (slider.value - slider.minimumValue) / (slider.maximumValue - slider.minimumValue)
                            Layout.fillWidth: true
                        }
                        ProgressBar {
                            indeterminate: true
                            Layout.fillWidth: true
                        }
                        Slider {
                            id: slider
                            value: 0.5
                            Layout.fillWidth: true
                            tickmarksEnabled: tickmarkCheck.checked
                            stepSize: tickmarksEnabled ? 0.1 : 0
                        }
                        MouseArea {
                            id: busyCheck
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            hoverEnabled:true
                            Layout.preferredHeight: busyIndicator.height
                            BusyIndicator {
                                id: busyIndicator
                                running: busyCheck.containsMouse
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                    ColumnLayout {
                        id: rightcol
                        Layout.fillWidth: true
                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                        }

                        GroupBox {
                            id: group1
                            title: "CheckBox"
                            Layout.fillWidth: true
                            RowLayout {
                                Layout.fillWidth: true
                                CheckBox {
                                    id: frameCheckbox
                                    text: "Text frame"
                                    checked: true
                                    Layout.minimumWidth: 100
                                }
                                CheckBox {
                                    id: tickmarkCheck
                                    text: "Tickmarks"
                                    checked: false
                                    Layout.minimumWidth: 100
                                }
                                CheckBox {
                                    id: wrapCheck
                                    text: "Word wrap"
                                    checked: true
                                    Layout.minimumWidth: 100
                                }
                            }
                        }
                        GroupBox {
                            id: group2
                            title:"Tab Position"
                            Layout.fillWidth: true
                            RowLayout {
                                ExclusiveGroup { id: tabPositionGroup }
                                RadioButton {
                                    id: r1
                                    text: "Top"
                                    checked: true
                                    exclusiveGroup: tabPositionGroup
                                    Layout.minimumWidth: 100
                                }
                                RadioButton {
                                    id: r2
                                    text: "Bottom"
                                    exclusiveGroup: tabPositionGroup
                                    Layout.minimumWidth: 100
                                }
                            }
                        }

                        TextArea {
                            id: area
                            frameVisible: frameCheckbox.checked
                            text: loremIpsum + loremIpsum
                            textFormat: Qt.RichText
                            wrapMode: wrapCheck.checked ? TextEdit.WordWrap : TextEdit.NoWrap
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            MouseArea {
                                id: contextMenu
                                parent: area.viewport
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton
                                onPressed: editmenu.popup()
                            }
                        }
                    }
                }
            }

        }
        Tab {
            title: "Itemviews"
        }
        Tab {
            title: "Styles"
        }
        Tab {
            title: "Layouts"
        }
    }
}

