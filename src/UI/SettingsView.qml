import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal

Rectangle {
    id: root
    color: '#444444'

    ColumnLayout {
        anchors.fill: parent
        spacing:10

        Text {
            text: "Settings"
            font.pointSize: 24
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        // ---------------- RowLayout with two buttons ----------------
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Button 1"
                Layout.preferredWidth: 150
                onClicked: console.log("Button 1 clicked")
            }

            Button {
                text: "Button 2"
                Layout.preferredWidth: 150
                onClicked: console.log("Button 2 clicked")
            }
        }

        ScrollView {
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            Layout.fillWidth: true
            Layout.fillHeight: true

            Column {                    
                Repeater {
                    id: channelRepeater
                    model: 16

                    // delegate: Rectangle {
                    //     width: 200
                    //     height: 50
                    //     color: '#ff0000'
                    // }
    
                    delegate: ChannelConfigurator_V2 {
                        ch_id: index
                        ch_current_value: SdlController.channelValues[index]
                        ch_min: 1000
                        ch_max: 2000
    
                        onApplyConfig: { SdlController.ApplyChannelSettings(index, selected_behaviour, value_offset) }
                        onClearConfig: { SdlController.ClearChannelConfig(index) }
                    }
                }
            }
        }
    }
}
