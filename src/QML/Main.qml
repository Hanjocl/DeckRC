import QtQuick 2.0
import QtQuick.Controls 2.0

Window {
    id: mainWindow

    Component.onCompleted: {
        mainWindow.showFullScreen()
        //mainWindow.showMaximized()
        window.forceActiveFocus()
    }

    width: 640
    height: 480
    visible: true
    title: qsTr("Deck RC")

    Button {
        text: "Quit"
        onClicked: Qt.quit()
    }
}
