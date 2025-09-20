import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {

    visibility: Window.FullScreen
    title: qsTr("DeckRC - Debug 0.1")
    Universal.theme: Universal.Dark

    property bool settingsOverlayVisible: false

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20

        Component.onCompleted: {
            inputArea.forceActiveFocus();
            SdlController.startPolling();
        }

        // Left column
        ColumnLayout {
            id: leftColumn
            Layout.preferredWidth: 200
            spacing: 10

            Button {
                text: "Capture"
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    SdlController.stopPolling();
                    console.log("capture function called");
                }
            }

            Button {
                text: "Record"
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    SdlController.startPolling();
                    console.log("record function called");
                }
            }

            Button {
                text: "RC Link"
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    inputArea.forceActiveFocus();
                    console.log("rc_link function called");
                }
            }
        }

        // Spacer / central input area
        Item {
            Layout.preferredWidth: parent.width * 0.9
            Layout.fillHeight: true

            Rectangle {
                id: inputArea
                width: parent.width
                height: parent.height
                border.color: "white"
                border.width: 3
                color: "transparent"
                anchors.margins: 10
                focus: true

                Keys.onPressed: function(event) {
                    SdlController.injectKey(event.key, event.text)
                }

                Rectangle {
                    anchors.fill: parent
                    color: "black"

                    Column {
                        anchors.centerIn: parent
                        spacing: 10

                        Text {
                            id: display
                            text: "Press a key"
                            font.pointSize: 22
                            color: "blue"
                        }
                    }
                }
            }

            SettingsView {
                id: settingsView
                width: parent.width
                height: parent.height
                visible : false
            }
        }

        // Right column
        ColumnLayout {
            id: rightColumn
            Layout.preferredWidth: 200
            spacing: 10

            Button {
                text: "Status"
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    console.log("status function called");
                }
            }

            Button {
                text: "Settings"
                Layout.fillWidth: true
                Layout.fillHeight: true
                checkable: true

                onClicked: {
                    settingsView.visible = !settingsView.visible;
                    inputArea.forceActiveFocus();
                    console.log("settings function called, overlayVisible:", settingsOverlayVisible);
                }
            }

            Button {
                text: "Shutdown"
                Layout.fillWidth: true
                Layout.fillHeight: true
                onClicked: {
                    console.log("Shutdown function called");
                    Qt.quit();
                }
            }
        }
    }
}
