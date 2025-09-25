import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal
import Qt.labs.platform

Rectangle {
    id: root
    color: '#ee444444'
    focus: true

    Keys.onPressed: function(event) {
        SdlController.injectKey(event.key, event.text)
    }

    property bool isConnected: false

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        anchors {
            topMargin: 20
            leftMargin: 50
            rightMargin: 50
            bottomMargin: 20
        }
        spacing:10

        Text {
            text: "Settings"
            font.pointSize: 24
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        RowLayout {
            // ---------------- CRSF Settings ----------------
            ColumnLayout {
                id: crsfLayout
                spacing: 10
    
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
    
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
    
                // Serial Device
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
                    checkable: true
                    
                    text: root.isConnected ? "Disconnect" : "Connect"
                    Layout.preferredHeight: 40
    
                    onClicked: {
                        if (!root.isConnected) {
                            CsrfApi.connect(serialDeviceComboBox.currentText, parseInt(baudRateComboBox.currentText))
                        } else {
                            CsrfApi.disconnect()
                        }
                    }

                    Connections {
                        target: CsrfApi

                        onConnected: root.isConnected = true;

                        onDisconnected: root.isConnected = false;
                    }
                }
    
                // Connection status text
                Text {
                    id: connectionStatus
                    text: ""
                    color: "lightgreen"
                    font.pixelSize: 16
    
                    Component.onCompleted: {
                        CsrfApi.isConnected();
                    }
                }
            }

            // ---------------- OpenIPC Settings ----------------
            ColumnLayout {
                Rectangle {
                    color: "red"
                    Layout.fillWidth: true   // fill horizontally
                    Layout.fillHeight: true  // fill vertically
                }

                ComboBox {
                    id: selectDev
                    width: 190
                    model: ListModel {
                        id: comboBoxModel
                        Component.onCompleted: {
                            var dongleList = NativeApi.GetDongleList();
                            for (var i = 0; i < dongleList.length; i++) {
                                comboBoxModel.append({text: dongleList[i]});
                            }
                            selectDev.currentIndex = 0; // Set default selection
                        }
                    }
                    currentIndex: 0
                }

                ComboBox {
                    id: selectCodec
                    width: parent.width
                    model: ['AUTO','H264','H265']
                    currentIndex: 0
                    Component.onCompleted: {
                        let codec = NativeApi.GetConfig()["config.codec"];
                        if (codec&&codec !== '') {
                            currentIndex = model.indexOf(codec);
                        }
                    }
                }

                ComboBox {
                    id: selectBw
                    width: parent.width
                    model: [ '20', '40', '80', '160', '80_80', '5', '10', 'MAX']
                    currentIndex: 0
                    Component.onCompleted: {
                        let chw = NativeApi.GetConfig()["config.channelWidth"];
                        if (chw&&chw !== '') {
                            currentIndex = Number(chw);
                        }
                    }
                }

                ComboBox {
                    id: selectChannel
                    width: parent.width
                    model: [
                        '1','2','3','4','5','6','7','8','9','10','11','12','13',
                        '32','36','40','44','48','52','56','60','64','68','96','100','104','108','112','116','120',
                        '124','128','132','136','140','144','149','153','157','161','169','173','177'
                    ]
                    currentIndex: 39
                    Component.onCompleted: {
                        let ch = NativeApi.GetConfig()["config.channel"];
                        if(ch&&ch!==''){
                            currentIndex = model.indexOf(ch);
                        }
                    }
                }

                Column {
                    FileDialog {
                        id: fileDialog
                        title: "Select key File"
                        nameFilters: ["Key Files (*.key)"]

                        onAccepted: {
                            keySelector.text = fileDialog.file;
                            keySelector.text = keySelector.text.replace('file:///','')
                            console.log("Key found at: ", keySelector.text)
                        }
                    }
                    Button {
                        width: 190
                        id:keySelector
                        text: "gs.key"
                        onClicked: fileDialog.open()
                        Component.onCompleted: {
                            let key = NativeApi.GetConfig()["config.key"];
                            if (key && key !== '') {
                                text = key;

                            }
                        }
                    }
                }

                Button { 
                    id: stream_starter
                    text: stream_started ? "Stop Stream" : "Start Stream";
                    property bool stream_started : false;
                    Layout.preferredWidth: 150;
                    checkable: true;
                    checked: stream_started;
                    onClicked: {
                        if (!stream_starter.stream_started) {
                            console.log("OpenIPC API: Trying to open stream")
                            stream_starter.stream_started = NativeApi.Start(
                                selectDev.currentText,
                                Number(selectChannel.currentText),
                                Number(selectBw.currentIndex),
                                keySelector.text,
                                selectCodec.currentText
                            );
                        } else {
                            NativeApi.Stop();
                            player.stop();
                        }
                    }

                    Component.onCompleted: {
                        NativeApi.onWifiStop.connect(()=>{
                            checked = false;
                            player.stop();
                        });

                        Qt.callLater(() => { // Waits for all other components initialize before commanding to start the stream!
                            console.log("OpenIPC API: Trying to open stream")
                            console.log("OpenIPC API: key (", keySelector.text, ")")
                            if (!stream_starter.stream_started && keySelector.text !== "gs.key") {
                                stream_starter.stream_started = NativeApi.Start(
                                    selectDev.currentText,
                                    Number(selectChannel.currentText),
                                    Number(selectBw.currentIndex),
                                    keySelector.text,
                                    selectCodec.currentText
                                );
                            }
                        })
                    }
                }                
            }
        }

        // ---------------- SDL Controller Settings ----------------
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

        /// ------ Channel Settings ------
        ScrollView {
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {   
                id: channelsColum  

                Repeater {
                    id: channelRepeater
                    model: 16
    
                    delegate: ChannelConfigurator_V2 {
                        ch_id: model.index
                        ch_current_value: SdlController.channelValues[model.index]
                        ch_min: 1000
                        ch_max: 2000
                        onScanForInputChanged: (checked) => channelsColum.handleInputCheckedChanged(ch_id, checked)
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
}
