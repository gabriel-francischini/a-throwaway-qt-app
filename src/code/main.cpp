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
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include <string>
#include <QFile>
#include "extra_fonts.hpp"


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

    // Since we'll use Quick Controls 2, opt-in for High-DPI devices
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    bool fontsLoadedOk = true;
    // Registers the extra embedded fonts
    for(const std::string& fontName : extra_fonts){
        const std::string fontFolder = ":/fonts/";
        const QString fontPath = (fontFolder + fontName).c_str();
        //qDebug() << "Loading font '" << fontPath << "'";
        const auto isOk = QFontDatabase::addApplicationFont(fontPath);
        //qDebug() << "    ^--> QRC file exists? " << QFile(fontPath).exists();
        //qDebug() << "    ^--> Result is: " << isOk;
        fontsLoadedOk = fontsLoadedOk && (isOk != -1);
        if(isOk != -1)
            for(const QString& family : QFontDatabase::applicationFontFamilies(isOk)){
                //qDebug() << "    ^--> Found family: '" << family << "'";
            }
    }

    if(fontsLoadedOk){
        QFont font("Roboto");
        QApplication::setFont(font);
    }

    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine(QUrl("qrc:///gui/mainwindow.qml"));

    // Entramos no loop de execução
    return a.exec();
}
