import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Controls.Universal 2.4
//import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    Material.theme: Material.Dark
    Universal.theme: Universal.Dark

    id: window
    visible: true
    width: 328
    height: 716

    //: Name of the initial screen
    title: qsTr("Demonstração de Resultado")

    Action {
        id: copyAction
        text: "&Copy"
        shortcut: StandardKey.Copy
        enabled: (!!activeFocusItem && !!activeFocusItem["copy"])
        onTriggered: activeFocusItem.copy()
    }

    Action {
        id: cutAction
        text: "Cu&t"
        shortcut: StandardKey.Cut
        enabled: (!!activeFocusItem && !!activeFocusItem["cut"])
        onTriggered: activeFocusItem.cut()
    }

    Action {
        id: pasteAction
        text: "&Paste"
        shortcut: StandardKey.Paste
        enabled: (!!activeFocusItem && !!activeFocusItem["paste"])
        onTriggered: activeFocusItem.paste()
    }

    menuBar: MenuBar {
        Menu {
            title: "Arquivo⊕⊖"
            MenuItem {
                text: "&Abrir"
            }

            MenuItem {
                text: "&Salvar"
            }
        }
        Menu {
            title: "Demonstrar"
            MenuItem {
                text: "Resultado Mensal"
            }

            MenuItem {
                text: "&Resultado Anual"
            }
            MenuItem {
                text: "Resultado de Período Específico"
            }
        }
    }

    ScrollView {
        id: scrollArea
        anchors.fill: parent
        Layout.fillWidth: true
        anchors {
            leftMargin: window.width / 10
            rightMargin: window.width / 10
            topMargin: window.height / 20
            bottomMargin: window.height / 20
        }

        ColumnLayout {
            id: centralLayout
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            anchors.fill: parent

            RowLayout {
                //Calendar {}
            }

            GroupBox {
                title: qsTr("Receitas - R$5.490,10")

                Label {
                    text: "Quantidade"
                }
            }
        }
    }
}
