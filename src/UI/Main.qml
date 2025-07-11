import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
     
    visibility: Window.FullScreen
    title: qsTr("DeckRC - Debug 0.1")

    Universal.theme: Universal.Dark
        
    ListModel {
        id: leftButtonsModel
        ListElement { text: "Capture"; actionId: "capture" }
        ListElement { text: "Record"; actionId: "record" }
        ListElement { text: "RC Link"; actionId: "rc_link" }
    }

    ListModel {
        id: rightButtonsModel
        ListElement { text: "status"; actionId: "status" }
        ListElement { text: "Settings"; actionId: "settings" }
        ListElement { text: "Shutdown"; actionId: "shutdown" }
    }

    property var actionFunctions: ({
        capture: function() { InputController.stopPolling(); console.log("capture function called"); },
        record: function() { InputController.startPolling(); console.log("record function called"); },
        rc_link: function() { InputController.stopScanning(); console.log("rc_link function called"); },
        status: function() { console.log("status function called"); },
        settings: function() { console.log("settings function called"); },
        shutdown: function() {
            console.log("Shutdown function called");
            Qt.quit();
        }
    })

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20

        // Left column sticks to the left
        ColumnLayout {
            id: leftColumn
            Layout.preferredWidth: 200

            Repeater {
                model: leftButtonsModel
                Button {
                    text: model.text
                    Layout.fillWidth: true
                    Layout.fillHeight: true


                    onClicked: {
                        if (actionFunctions[actionId]) {
                            actionFunctions[actionId]()
                        }
                    }
                }
            }
        }

        // Spacer fills all the space between left and right columns
        Item {
            Layout.preferredWidth: parent.width *.9
            Layout.fillHeight: true

            Rectangle {
                width: parent.width
                height: parent.height
                border.color: "white"
                color: "transparent"
                border.width: 3
                anchors.margins: 10

                ColumnLayout {
                    id: root_channels
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: parent.height
                    anchors {
                        fill: parent
                    }

                    function handleInputCheckedChanged(checked) {
                        console.log("Checked state changed:", checked)
                        if (checked) {
                            InputController.GetInput();
                        } else {
                            InputController.stopScanning();
                            InputController.startPolling();
                        }
                    }

                    Repeater {
                        id: channelRepeater
                        model: 16
    
                        ChannelSettings {
                            Layout.alignment: Qt.AlignHCenter
                            id: settings
                            ch_id: model.index    
                            ch_current_value: InputController.channelValues[model.index]
                            ch_min: 1000
                            ch_max: 2000
                            onInputCheckedChanged: (checked) => root_channels.handleInputCheckedChanged(checked)
                        }

                        
                    }
                }

            }            
        }         

        // Right column sticks to the right
        ColumnLayout {
            id: rightColumn
            Layout.preferredWidth: 200
            spacing: 10

            Repeater {
                model: rightButtonsModel
                Button {
                    text: model.text
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    onClicked: {
                        if (actionFunctions[actionId]) {
                            actionFunctions[actionId]()
                        }
                    }
                }
            }
        }
    }
}
