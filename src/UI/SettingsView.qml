import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "#f0f0f0"
    radius: 10
    border.color: "black"
    border.width: 2
    anchors.fill: parent
    opacity: 1.0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Text {
            text: "Settings"
            font.pointSize: 24
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                id: settingsColumn
                spacing: 5

                Repeater {
                    id: channelRepeaterSettings
                    model: 16
                    ChannelSettings {
                        ch_id: model.index
                        ch_current_value: InputController.channelValues[model.index]
                        ch_min: 1000
                        ch_max: 2000
                        onInputCheckedChanged: (checked) => {
                            if (checked) {
                                InputController.GetInput();
                            } else {
                                InputController.stopScanning();
                                InputController.startPolling();
                            }
                        }
                    }
                }
            }
        }

        Button {
            text: "Close"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                root.parent.visible = false
            }
        }
    }
}
