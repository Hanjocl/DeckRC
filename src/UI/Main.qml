import QtQuick
import QtQuick.Controls.Universal
import QtQuick.Controls
import QtQuick.Layouts

import realTimePlayer 1.0


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

            CaptureButton{
                id: captureButton
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RecordButton {
                id: myRecorder
                Layout.fillWidth: true
                Layout.fillHeight: true
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
                z: 0

                Keys.onPressed: function(event) {
                    SdlController.injectKey(event.key, event.text)
                }
            }

            Column {
                id: logging
                anchors.centerIn: parent
                spacing: 10
                z: 1
                visible: true

                // Default display / placeholder text
                Text {
                    id: latestMessage
                    text: "Status unknown"
                    font.pointSize: 18
                    color: "white"
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // Latest log message
                Text {
                    id: latestLog
                    text: ""
                    font.pointSize: 14
                    color: "grey"
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.Wrap
                }

                Component.onCompleted: {
                    // Connect log messages
                    NativeApi.onLog.connect((level, msg) => {
                        latestLog.text = msg

                        if (msg.includes("Creating Rtl8812aDevice")) {
                            latestMessage.text = "RTL8812AU found"
                        }

                        if (msg.includes("Listening")) {
                            latestMessage.text = "Waiting for stream"
                        }

                        if (msg.includes("=stoped=")) {
                            latestMessage.text = "Stream stopped"
                        }
                    });
                }
            }

            QQuickRealTimePlayer {
                z: 1
                id: player
                anchors.fill: parent
                property var playingFile
                Component.onCompleted: {
                    NativeApi.onRtpStream.connect((sdpFile)=>{
                        playingFile = sdpFile;
                        play(sdpFile)
                        logging.visible = false
                        visible = true
                    });
                    onPlayStopped.connect(()=>{
                        stop();
                        play(playingFile)
                        logging.visible = true
                        visible = false
                    });
                }
            }

            Row {
                spacing: 10
                z: 2
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: 5

                // Bitrate display
                Text {
                    id: bitrateText
                    text: "0bps"
                    font.pixelSize: 14
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter

                    Component.onCompleted: {
                        player.onBitrate.connect((btr)=>{
                            if(btr > 1000*1000){
                                text = Number(btr/1000/1000).toFixed(2) + "Mbps";
                            } else if(btr > 1000){
                                text = Number(btr/1000).toFixed(2) + "Kbps";
                            } else {
                                text = btr + "bps";
                            }
                        });
                    }
                }

                // RC Link Status
                Text {
                    id: rcLinkStatus
                    textFormat: Text.RichText
                    font.pixelSize: 14

                    // Fixed part
                    property string prefix: "<font color='white'>Control Link TX:</font> "
                    // Dynamic part
                    property string status: "Unknown"

                    text: prefix + "<font color='white'>" + status + "</font>"

                    Connections {
                        target: CsrfApi

                        onConnected: {
                            rcLinkStatus.status = "<font color='green'>Connected</font>"
                            log.console("Testing") 
                        }
                            onDisconnected: rcLinkStatus.status = "<font color='red'>Disconnected</font>"
                    }

                    Component.onCompleted: {
                        if (CsrfApi.isConnected()) {
                            rcLinkStatus.status = "<font color='green'>Connected</font>"
                        } else {
                            rcLinkStatus.status = "<font color='white'>Unknown</font>"
                        }
                        rcLinkStatus.text = rcLinkStatus.prefix + rcLinkStatus.status
                    }

                    onStatusChanged: rcLinkStatus.text = rcLinkStatus.prefix + rcLinkStatus.status
                }
            }


            SettingsView {
                z:3
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
