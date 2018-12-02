// Bibliotecas de terceiros

// Bibliotecas dessse projeto
// #include "lib/janelas.hpp"
// #include "lib/auxiliar.hpp"
// #include "lib/database_api.hpp"
//#include "gui/mainwindow/mainwindow.h"

// Bibliotecas do QT
#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QDebug>
#include <QQuickView>
#include <QUrl>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>


// Todo código em C/C++ é iniciado pela função main
int main(int argc, char *argv[])
{

    // Definimos uma QApplication (necessária para
    // iniciar o Qt e suas ferramentas)
    QApplication a(argc, argv);

    // Aplicamos um estilo mais escuro
    //CDarkStyle::assign();

    //MainWindow janela_principal{};
    //qDebug() << "O endereço da janela principal é " << (int *) &janela_principal;

    // Por último, iniciamos alguns set-ups
    // Auxiliar::setUp(janela_principal);

    //janela_principal.show();

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine(QUrl("qrc:///gui/mainwindow.qml"));

    // Entramos no loop de execução
    return a.exec();
}
