import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

RowLayout {
    id: ch_settings
    height:100
   
    property real ch_id: -1
    property real ch_current_value: 69
    property real ch_min: -100
    property real ch_max: 100

    enum Behaviours {
        Toggle,
        Toggle_sym,
        Tap,
        Raw_Axis
    }

    property var behaviour_types: ["Toggle", "Toggle (Symetrical)", "Tap", "Raw Axis"];
    property var selected_behaviour: -1;
    property int target_value: 0;
    property string input_label: "" 
    signal inputCheckedChanged(bool checked)
    
    ColumnLayout {
        id: colum_base
        Layout.alignment: Qt.AlignLeft
        spacing : 10

        Text {
            id: channel_id
            text: `Channel ${ch_settings.ch_id}`
            color: Universal.baseHighColor
            font.pointSize: 16
            Layout.alignment: Qt.AlignTop

            
        }

        Item {
            id: progressBar
            Layout.preferredWidth: 400
            width:parent.width
            
            ProgressBar {
                id: raw_progressBar
                anchors.centerIn: parent
                width:parent.width             

                from: ch_settings.ch_min
                to: ch_settings.ch_max
                value: ch_settings.ch_current_value
            }

            // display min bound
            Text {
                anchors.left: raw_progressBar.left
                text: ch_settings.ch_min
                color: Universal.baseMediumHighColor
                font.pointSize: 12
            }

            // display current value
            Text {
                anchors.top: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: ch_settings.ch_current_value
                color: Universal.baseHighColor
                font.weight: Font.Bold
                font.pointSize: 12
            }

            // display max bound
            Text {
                anchors.right: raw_progressBar.right
                text: ch_settings.ch_max
                color: Universal.baseMediumHighColor
                font.pointSize: 12
            }

        }
    }    

    Button {
        id: input
        Layout.preferredHeight: colum_base.height
        Layout.preferredWidth: 200       
        text: checked ? "Waiting for input..." : ch_settings.input_label === "" ? "Set input" : ch_settings.input_label
        checkable: true
        onCheckedChanged: {
            inputCheckedChanged(checked)
        }
    }
    
    ComboBox {
        id: comboBox
        Layout.preferredHeight: colum_base.height
        displayText: ch_settings.selected_behaviour === -1 ? "Select Behaviour" : model[currentIndex]
        currentIndex: ch_settings.selected_behaviour
        model: ch_settings.behaviour_types
        onCurrentIndexChanged: {
            ch_settings.selected_behaviour = currentIndex
        }
    }

    RowLayout {
        spacing: 10
        Layout.preferredHeight: colum_base.height


        Button {
            Layout.preferredHeight: colum_base.height
            text: "-"
            onClicked: {
                ch_settings.target_value -= 100
            }
            Accessible.name: "Decrease target value"
            Accessible.description: "Decreases target value by 25"
        }

        TextInput {
            id: target_value
            Layout.preferredWidth: 50
            text: `${ch_settings.target_value}`
            font.pixelSize: 20
            color: Universal.baseHighColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            inputMethodHints: Qt.ImhDigitsOnly

            onTextChanged: {
                const val = parseInt(text)
                if (!isNaN(val))
                    ch_settings.target_value = val
            }

            // Optionally: prevent non-numeric input
            validator: IntValidator { bottom: ch_settings.ch_min; top: ch_settings.ch_max }
        }

        Button {
            text: "+"
            onClicked: {
                ch_settings.target_value += 100
            }
            Accessible.name: "Increase target value"
            Accessible.description: "Increases target value by 25"
        }
    }

    Button {
        id: apply
        Layout.preferredHeight: colum_base.height
        text: "Apply"
    }
    
    Button {
        id: clear
        Layout.preferredHeight: colum_base.height
        text: qsTr("Clear")
        onClicked: {
            // TODO -> Send clear command to cpp
            ch_settings.selected_behaviour = -1
            ch_settings.target_value = 0
            ch_settings.input_label = ""
            input.checked = false
        }
    }
}