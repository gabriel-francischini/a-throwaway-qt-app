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
    title: qsTr("App Modelo Organizacional")

    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        Component.onCompleted: {console.log("columnlayout0 wh: ", width, height);}

        TabBar {
            id: tabBar
            currentIndex: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            //Layout.preferredHeight: window.height * 0.1
            //Layout.minimumHeight: window.height * 0.1
            Component.onCompleted: {console.log("tabBar wh: ", width, height);}

            TabButton {
                id: entryButton
                text: qsTr("Entrada")
                Component.onCompleted: {console.log("entryButton wh: ", width, height);}
            }

            TabButton {
                id: reportBurron
                text: qsTr("Relatório")
                Component.onCompleted: {console.log("reportButton wh: ", width, height);}
            }
        }

        ScrollView {
            id: scrollView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: window.height * 0.9
            Component.onCompleted: {console.log("scrollView wh: ", width, height);}
            contentItem: columnLayout

            ColumnLayout {
                id: columnLayout
                width: scrollView.width
                //height: scrollView.height
                Component.onCompleted: {console.log("columnLayout (1) wh: ", width, height);}
                StackLayout {
                    id: tabContentStacker
                    currentIndex: tabBar.currentIndex
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: window.height * 0.05
                    Layout.bottomMargin: window.height * 0.05
                    Layout.leftMargin: window.width * 0.05
                    Layout.rightMargin: window.width * 0.05
                    //Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Component.onCompleted: {console.log("stacklayout wh: ", width, height);}
                    ColumnLayout {
                        id: entryMasterLayout
                        spacing: 0
                        Layout.fillWidth: true
                        Component.onCompleted: {console.log("columnlayout2 wh: ", width, height);}
                        GroupBox {
                            id: entryGroupBox
                            Component.onCompleted: {console.log("entryGroupBox wh: ", width, height);}
                            Layout.fillWidth: false
                            title: qsTr("Adicionar movimentação")
                            contentItem: gBoxLayout

                            ColumnLayout {
                                spacing: 10
                                id: gBoxLayout

                                Label {
                                    text: "Quantidade"
                                }
                                Rectangle {
                                    height: window.height * 2
                                    //Layout.fillWidth: true
                                    width: window.width * 2
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: "#FF5252" }
                                        GradientStop { position: 1.0; color: "lightsteelblue" }
                                    }
                                    Component.onCompleted: {console.log("rectangleReceitas wh: ", width, height);}
                                }
                            }
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
    }
}
