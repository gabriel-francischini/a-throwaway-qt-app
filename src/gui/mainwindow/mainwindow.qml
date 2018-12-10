//import QtQuick 2.9
import QtQuick 2.11
import QtQuick.Window 2.3
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
//import QtQuick.Dialogs 1.11
//import QtQuick.Controls 1.5
//import QtQuick.Controls 2.4
import QtQuick.Controls 2.4
//import QtQuick.Controls.Material 2.4
//import QtQuick.Controls.Universal 2.4
//import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.4
import QtQuick.Controls.Universal 2.4
import QtQuick.Controls.Styles 1.4

import App.Validators 1.0

ApplicationWindow {
    Material.theme: Material.Dark
    //Universal.theme: Universal.Dark

    FontLoader { id: emojiOneColor; source: "qrc:/fonts/EmojiOneColor.otf" }

    id: window
    visible: true

    property real lesser_phi: 0.61803398875 // 1/phi, i.e. phi in %
    property real height_screen_proportion: 0.6 // 60%

    width: Math.round(Screen.height * height_screen_proportion * lesser_phi)
    height: Math.round(Screen.height * height_screen_proportion)

    //: Name of the initial screen
    title: qsTr("Modelo Organizacional")

    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        Component.onCompleted: {console.log("masterWindowLayout whxy: ", width, height, x, y);}

        TabBar {
            id: tabBar
            currentIndex: 0
            Layout.fillWidth: true
            //Layout.fillHeight: true
            //Layout.preferredHeight: window.height * 0.1
            //Layout.minimumHeight: window.height * 0.1
            Component.onCompleted: {console.log("tabBar whxy: ", width, height, x, y);}

            TabButton {
                id: entryButton
                text: qsTr("Entrada")
                Component.onCompleted: {console.log("entryButton whxy: ", width, height, x, y);}
            }

            TabButton {
                id: reportBurron
                text: qsTr("Relatório")
                Component.onCompleted: {console.log("reportButton whxy: ", width, height, x, y);}
            }
        }

        StackLayout {
            id: tabContentStacker
            currentIndex: tabBar.currentIndex
            //Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: window.height * 0.05
            Layout.bottomMargin: window.height * 0.05
            Layout.leftMargin: window.width * 0.05
            Layout.rightMargin: window.width * 0.05
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Component.onCompleted: {console.log("stacklayout whxy: ", width, height, x, y);}
            ColumnLayout {
                id: entryMasterLayout
                spacing: scrollView.height * 0.05
                Layout.fillWidth: true
                Component.onCompleted: {console.log("insideStackColumnLayout whxy: ", width, height, x, y);}
                GroupBox {
                    id: entryGroupBox
                    Component.onCompleted: {console.log("entryGroupBox whxy: ", width, height, x, y);}
                    Layout.fillWidth: true
                    title: qsTr("Adicione uma nova movimentação")
                    //font.bold: true

                    contentItem: gBoxLayout

                    GridLayout {
                        columns: 2
                        id: gBoxLayout
                        //anchors.fill: entryGroupBox
                        Layout.fillWidth: true
                        Component.onCompleted: {console.log("gBoxLayout whxy: ", width, height, x, y);}
                        columnSpacing: window.width * 0.05
                        Label {
                            id: categoryLabel
                            Layout.fillWidth: true
                            text: qsTr("Categoria")
                            Component.onCompleted: {console.log("categoryLabel whxy: ", width, height, x, y);}
                        }

                        ComboBox {
                            id: categoryField
                            Layout.fillWidth: true
                            Component.onCompleted: {console.log("categoryField whxy: ", width, height, x, y);}
                            model: [
                                qsTr("Administração"),
                                qsTr("Marketing"),
                                qsTr("Vendas"),
                                qsTr("Recursos Humanos"),
                                qsTr("Despesas Financeiras"),
                                qsTr("Materiais"),
                                qsTr("Outros")
                            ]
                        }

                        Label {
                            id: dateLabel
                            Layout.fillWidth: true
                            Component.onCompleted: {console.log("dateLabel whxy: ", width, height, x, y);}
                            text: qsTr("Data")
                        }
                        TextField {
                            id: dateField
                            Layout.fillWidth: true
                            Component.onCompleted: {console.log("dateField whxy: ", width, height, x, y);}
                            placeholderText: qsTr("05/08/2018")
                            onTextChanged: {
                                if(DateValidator.isDateValid(text)){
                                    console.log("text is: '" + text + "'");
                                }
                            }
                        }

                        Label {
                            id: descLabel
                            Layout.fillWidth: true
                            Component.onCompleted: {console.log("descLabel whxy: ", width, height, x, y);}
                            text: qsTr("Descrição")
                        }

                        ScrollView {
                            id: scrollView
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: window.height * 0.25
                            Layout.preferredWidth: window.width * 0.10
                            clip: true
                            Component.onCompleted: {console.log("scrollView whxy: ", width, height, x, y);}
                            contentItem: descField

                            TextArea {
                                id: descField
                                //anchors.fill: parent
                                //Layout.fillWidth: true
                                Component.onCompleted: {console.log("descField whxy: ", width, height, x, y);}
                                placeholderText: qsTr("Insira uma descrição")
                            }
                        }
                    }
                }

                Button {
                    id: submitButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: window.height * 0.1
                    text: qsTr("Adicionar")
                    Component.onCompleted: {console.log("submitButton whxy: ", width, height, x, y);}
                }
            }

            ColumnLayout {
                id: reportMasterLayout
                spacing: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
                Rectangle {
                    Layout.preferredHeight: 50
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "#AA00FF"
                    opacity: 0
                    //title: qsTr("Relatórios")
                }
            }
        }

    }
}


