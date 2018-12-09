//import QtQuick 2.9
import QtQuick 2.11
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

     id: window
     visible: true
     width: 370
     height: 598

     //: Name of the initial screen
     title: qsTr("App Modelo Organizacional")

    Component.onCompleted: {console.log("window wh: ", width, height);}


    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        Component.onCompleted: {console.log("columnlayout0 wh: ", width, height);}

        Rectangle {
            id: tabBar
            //currentIndex: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 20
            color: "#3F51B5"
            Component.onCompleted: {console.log("tabBar wh: ", width, height);}
        }

        ScrollView {
            id: scrollView
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            Component.onCompleted: {console.log("scrollView wh: ", width, height);}
            contentItem: columnLayout

            ColumnLayout {
                id: columnLayout
                anchors.fill: parent
                Component.onCompleted: {console.log("columnLayout (1) wh: ", width, height);}
                StackLayout {
                    currentIndex: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    //Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    Component.onCompleted: {console.log("stacklayout wh: ", width, height);}
                    ColumnLayout {
                        spacing: 0
                        Layout.fillWidth: true
                        Component.onCompleted: {console.log("columnlayout2 wh: ", width, height);}
                        Rectangle {
                            //Layout.preferredHeight: 50
                            //Layout.fillHeight: true
                            height: window.height
                            Layout.fillWidth: true
                            //color: "#FF5252"
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "#FF5252" }
                                GradientStop { position: 1.0; color: "lightsteelblue" }
                            }
                            //title: qsTr("Receitas - R$5.490,10")
                            Component.onCompleted: {console.log("rectangleReceitas wh: ", width, height);}
                        }
                    }

                    ColumnLayout {
                        spacing: 0
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Rectangle {
                            Layout.preferredHeight: 50
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#AA00FF"
                            //title: qsTr("Relatórios")
                        }
                    }
                }

            }
        }
    }
}
