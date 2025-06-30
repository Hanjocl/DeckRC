import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

RowLayout {
    id: ch_settings
    height:100
   
    property int ch_id: -1
    property int ch_current_value: 1500
    property int ch_min: 1000
    property int ch_max: 2000

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
            Layout.preferredWidth: 200
            
            ProgressBar {
                id: raw_progressBar
                anchors.fill: parent
                height: parent.height

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
                anchors.centerIn: raw_progressBar
                text: raw_progressBar.value
                color: Universal.baseHighColor
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
    }
    
    ComboBox {
        id: comboBox
        Layout.preferredHeight: colum_base.height

        displayText: ch_settings.selected_behaviour === -1  ? "Select Behaviour" : model[currentIndex]
        // onActivated: backend.modifier = currentValue         // TODO: Retrive value from back end
        Component.onCompleted: currentIndex = ch_settings.selected_behaviour
        currentIndex: ch_settings.selected_behaviour
        model: ch_settings.behaviour_types

        onCurrentIndexChanged: {
            ch_settings.selected_behaviour = currentIndex
            console.log("Selected behaviour enum value:", model[currentIndex]);
        }
    }

    RowLayout {
        spacing: 10
        Layout.preferredHeight: colum_base.height


        Button {
            Layout.preferredHeight: colum_base.height
            text: "-"
            onClicked: {
                ch_settings.target_value -= 25
            }
            Accessible.name: "Decrease target value"
            Accessible.description: "Decreases target value by 25"
        }

        TextInput {
            id: target_value
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
                ch_settings.target_value += 25
            }
            Accessible.name: "Increase target value"
            Accessible.description: "Increases target value by 25"
        }
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