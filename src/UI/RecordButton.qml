// RecordButton.qml
import QtQuick 2.12
import QtQuick.Controls 2.5

Button {
    id: recorder
    property bool started: false
    property var startTime: 0
    property var recordLen: 0

    // Custom content with blinking red dot + text
    contentItem: Item {
        anchors.fill: parent

        Column {
            anchors.centerIn: parent
            spacing: 2

            // First line
            Text {
                id: line1
                text: "Record"
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Second line: red dot + elapsed time
            Row {
                spacing: 4
                anchors.horizontalCenter: parent.horizontalCenter
                height: Math.max(line2.font.pixelSize, redCyc.font.pixelSize)

                Text {
                    id: redCyc
                    text: "●" 
                    color: "red"
                    opacity: 0
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    id: line2
                    text: ""
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }



    // Timer to update elapsed time
    Timer {
        id: timer
        interval: 100
        running: false
        repeat: true
        onTriggered: {
            recorder.recordLen = Math.floor((new Date().getTime() - recorder.startTime) / 1000)
            line2.text = recorder.recordLen + "S"
        }
    }

    // Timer for red dot blinking
    Timer {
        id: redCycTimer
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            redCyc.opacity = redCyc.opacity === 1 ? 0 : 1
        }
    }

    function startRecording() {
        if (player && !recorder.started) {
            recorder.started = player.startRecord()
            if (recorder.started) {
                recorder.startTime = new Date().getTime()
                redCyc.visible = true
                timer.start()
                redCycTimer.start()
                line1.text = "Recording"
                line2.text = "0S"
            }
        } else {
            stopRecording()
        }
    }

    function stopRecording() {
        recorder.started = false
        timer.stop()
        redCycTimer.stop()
        redCyc.visible = false
        recorder.recordLen = 0
        line1.text = "Record"
        line2.text = ""
        if (player) player.stopRecord()
    }

    onClicked: {
        if (!recorder.started) {
            startRecording()
        } else {
            stopRecording()
        }
    }
}
