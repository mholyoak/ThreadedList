import QtQuick 2.9
import QtQuick.Window 2.2


Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Threaded List")

    ToDoList {
        anchors.centerIn: parent
    }

    /*
    ListModel {
        id: threadedListModel

        ListElement {
            name: "Bill Smith"
        }
        ListElement {
            name: "John Brown"
        }
        ListElement {
            name: "Sam Wise"
        }
    }

    ListView {
        id: threadedListId
        anchors.fill: parent

        model: threadedListModel
        delegate: Text { text: name }
    }
    */
}
