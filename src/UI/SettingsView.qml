import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal

Rectangle {
    id: root
    color: '#444444'
    Layout.fillWidth: true
    Layout.fillHeight: true

    property bool isConnected: false

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        anchors.margins: 20
        spacing: 10

        Text {
            text: "Settings"
            font.pointSize: 24
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        // ---------------- Channel Config & Polling Controls ----------------
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.margins: 10
            spacing: 10

            // Load / Save Config Buttons
            RowLayout {
                spacing: 10
                Layout.fillWidth: true

                Button {
                    text: "Load Config"
                    Layout.preferredWidth: 150
                    onClicked: CsrfApi.loadConfig()
                }

                Button {
                    text: "Save Config"
                    Layout.preferredWidth: 150
                    onClicked: CsrfApi.saveConfig()
                }
            }
            
            // Serial Device ComboBox + Refresh
            RowLayout {
                spacing: 10
                Layout.fillWidth: true

                ComboBox {
                    id: serialDeviceComboBox
                    Layout.preferredWidth: 250
                    model: CsrfApi.listSerialPorts()
                    currentIndex: 0
                    Layout.preferredHeight: 40
                }

                Button {
                    text: "⟳"
                    width: 40
                    height: 40
                    ToolTip.text: "Refresh Device List"
                    onClicked: serialDeviceComboBox.model = CsrfApi.listSerialPorts()
                }
            }

            // Baud Rate ComboBox
            ComboBox {
                id: baudRateComboBox
                Layout.preferredWidth: 300
                model: ["9600","19200","38400","57600","115200","230400","460800","921600"]
                currentIndex: 7
                Layout.preferredHeight: 40
            }

            // Connect / Disconnect Button
            Button {
                id: connectButton
                text: isConnected ? "Disconnect" : "Connect"
                Layout.preferredWidth: 300
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 40

                onClicked: {
                    if (!isConnected) {
                        CsrfApi.connect(serialDeviceComboBox.currentText, parseInt(baudRateComboBox.currentText))
                    } else {
                        CsrfApi.disconnect()
                    }
                }
            }

            // Connection status text
            Text {
                id: connectionStatus
                text: ""
                color: "lightgreen"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter

                Component.onCompleted: {
                    CsrfApi.isConnected();
                }
            }

            // CsrfApi Connections
            Connections {
                target: CsrfApi
                onConnected: {
                    connectionStatus.text = "Connected to " + serialDeviceComboBox.currentText
                    connectionStatus.color = "lightgreen"
                    isConnected = true
                }
                onDisconnected: function(portName, message) {
                    connectionStatus.text = message
                    connectionStatus.color = "red"
                    isConnected = false
                }
                onConfigLoaded: {
                    serialDeviceComboBox.model = CsrfApi.listSerialPorts()
                    serialDeviceComboBox.currentIndex = serialDeviceComboBox.model.indexOf(CsrfApi.getPort())
                    baudRateComboBox.currentIndex = baudRateComboBox.model.indexOf(CsrfApi.getBaudRate().toString())
                }
            }
        }


        // ---------------- Channel Config & Polling Controls ----------------
        Rectangle {
            id: root_channels
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#2b2b2b"
            focus: true

            Keys.onPressed: function(event) {
                SdlController.injectKey(event.key, event.text)
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10
                Layout.margins: 10

                // ---------------- Top row: Load/Save + Polling ----------------
                RowLayout {
                    spacing: 10
                    Layout.fillWidth: true

                    Button { text: "Load Config"; Layout.preferredWidth: 150; onClicked: SdlController.loadConfig() }
                    Button { text: "Save Config"; Layout.preferredWidth: 150; onClicked: SdlController.saveConfig() }
                    Button { text: "Start Polling"; Layout.preferredWidth: 150; onClicked: SdlController.startPolling() }
                    Button { text: "Stop Polling"; Layout.preferredWidth: 150; onClicked: SdlController.stopPolling() }

                    TextInput {
                        id: pollingRateInput
                        property int pollingRate: 100
                        text: pollingRate.toString()
                        Layout.minimumWidth: 50
                        font.pixelSize: 20
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        validator: IntValidator { bottom: 50; top: 250 }
                        onEditingFinished: {
                            const val = parseInt(text, 10)
                            if (!isNaN(val)) {
                                pollingRate = val
                                text = val.toString()
                            } else {
                                text = pollingRate.toString()
                            }
                        }
                    }
                }

                // ---------------- Scrollable channels ----------------
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // Use Column instead of ColumnLayout
                    Column {
                        id: channelsColumn
                        width: parent.width
                        spacing: 10

                        Repeater {
                            id: channelRepeater
                            model: 16

                            ChannelConfigurator_V2 {
                                ch_id: model.index
                                ch_current_value: SdlController.channelValues[model.index]
                                ch_min: 1000
                                ch_max: 2000
                                onScanForInputChanged: (checked) => root_channels.handleInputCheckedChanged(ch_id, checked)
                                onApplyConfig: { SdlController.ApplyChannelSettings(ch_id, selected_behaviour, value_offset) }
                                onClearConfig: { SdlController.ClearChannelConfig(ch_id) }
                            }

                            Component.onCompleted: {
                                for (var i = 0; i < channelRepeater.count; i++) {
                                    var child = channelRepeater.itemAt(i)
                                    if (child) {
                                        child.input_label = SdlController.getChannelInputLabel(i)
                                        child.checked = false
                                        child.selected_behaviour = SdlController.getMode(i)
                                        child.value_offset = SdlController.getChannelOffset(i)
                                    }
                                }
                            }
                        }

                        Connections {
                            target: SdlController
                            onConfigLoaded: {
                                for (var i = 0; i < channelRepeater.count; i++) {
                                    var child = channelRepeater.itemAt(i)
                                    if (child) {
                                        child.input_label = SdlController.getChannelInputLabel(i)
                                        child.checked = false
                                        child.selected_behaviour = SdlController.getMode(i)
                                        child.value_offset = SdlController.getChannelOffset(i)
                                    }
                                }
                            }
                        }
                    }
                }
            }

            function handleInputCheckedChanged(ch_id, checked) {
                var child = channelRepeater.itemAt(ch_id)
                if (!child) return

                if (checked) {
                    child.input_label = ""
                    Qt.callLater(function() {
                        var inputText = SdlController.getInput(ch_id)
                        if (inputText && inputText.length > 0) {
                            child.input_label = inputText
                        }
                        child.checked = false
                    })
                } else {
                    SdlController.stopScanning()
                }

                root_channels.forceActiveFocus()
            }
        }

    }
}