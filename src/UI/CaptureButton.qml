// CaptureButton.qml
import QtQuick 2.12
import QtQuick.Controls 2.5

Button {
    id: captureBtn

    // Custom content
    contentItem: Item {
        anchors.fill: parent

        Column {
            anchors.centerIn: parent
            spacing: 2

            // First line
            Text {
                id: line1
                text: "Capture"
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Second line: status message
            Text {
                id: line2
                text: ""
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    // Timer to reset the message
    Timer {
        id: resetTimer
        interval: 1000 // 1 second
        running: false
        repeat: false
        onTriggered: line2.text = ""
    }

    // Click logic
    onClicked: {
        if (player) {
            let f = player.captureJpeg()
            if (f !== '') {
                line2.text = "Saved"
                resetTimer.start()
                console.log("Saved: " + f)
            } else {
                line2.text = "Capture failed!"
                resetTimer.start()
                console.log("Capture failed!")
            }
        }
    }
}
