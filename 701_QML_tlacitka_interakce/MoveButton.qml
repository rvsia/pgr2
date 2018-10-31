import QtQuick 2.0

Rectangle {
    property alias buttonText: textLabel.text
    id: myButton
    anchors.horizontalCenter: parent.horizontalCenter
    y: parent.height - 60
    color: Qt.rgba(0.5, 0.5, 0.5, 1.0)
    width: 50
    height: 50
    radius: 25
    border.width: 2
    border.color: "white"

    Text{
        id: textLabel
        anchors.centerIn: parent
        text: ">"
        color: "white"
        font {pixelSize: 25; bold:true; family: "Arial"}
    }

    states:[
        State{
            name:"Highlited"
            PropertyChanges {
                target: myButton
                border.color: "red"
            }
            PropertyChanges {
                target: textLabel
                color: "red"
            }
        }
    ]
}
