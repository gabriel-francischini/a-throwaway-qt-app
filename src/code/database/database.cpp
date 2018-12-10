#include "database/database.hpp"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QDir>
#include <QApplication>
#include <QVariant>
#include <QQmlEngine>

#include "transaction/transaction.hpp"

QSqlDatabase Database::Database::db;
bool Database::Database::dbInitialized = false;

namespace Database {
    void Database::startDb(){
        db = QSqlDatabase::addDatabase("QSQLITE", "db");
        db.setDatabaseName(qApp->applicationDirPath()
                           + QDir::separator()
                           + "info.stdb");
        // Makes sure we have a db connection
        if(!db.open() || qApp == nullptr){
            qDebug() << "An error occured while opening the database:";
            qDebug() << db.lastError();
            abort();
        } else {
            // Do use foreign keys
            QSqlQuery("PRAGMA foreign_keys = 1", db);
        }

        // dirty flag indicating that the database has been opened
        dbInitialized = true;

        QSqlQuery query(db);
        if(query.prepare(createCommand))
            query.exec();
        flushErrors(query);
    }

    bool Database::addTransaction(const QString& moneyStr, const QString& dateStr,
                                  const QString& descStr, const QString& typeStr){
        if(!dbInitialized) startDb();
        Transaction transaction;
        transaction.desc = descStr;
        transaction.type = typeStr;
        transaction.setMoney(moneyStr);
        transaction.setDate(dateStr);

        QString queryString =
            "INSERT INTO transactions "
            "(description, money, type, day, month, year) "
            "VALUES (:desc, :cash, :type, :day, :mon, :year);";

        QSqlQuery query(db);
        query.prepare(queryString);
        query.bindValue(":desc", transaction.desc);
        query.bindValue(":cash", transaction.money);
        query.bindValue(":type", transaction.type);
        query.bindValue(":day", transaction.day);
        query.bindValue(":mon", transaction.month);
        query.bindValue(":year", transaction.year);

        auto result = query.exec();
        flushErrors(query);
        return result;
    }

    double Database::sumByMonthAndType(const QString& dateStr,
                                       const QString& typeStr){
        if(!dbInitialized) startDb();
        Transaction transaction;
        transaction.setDate(dateStr);
        transaction.type = typeStr;

        QString sqlString =
            "SELECT SUM(money) FROM transactions "
            "WHERE month = :mon AND year = :year AND type = :type;";

        QSqlQuery query(db);
        query.prepare(sqlString);
        query.bindValue(":mon", transaction.month);
        query.bindValue(":year", transaction.year);
        query.bindValue(":type", transaction.type);

        double result = -1;
        query.exec();
        if(!flushErrors(query)){
            query.first();
            result = query.value(0).toDouble();
        }
        return result;
    }


    double Database::sumByMonth(const QString& dateStr){
        if(!dbInitialized) startDb();
        Transaction transaction;
        transaction.setDate(dateStr);

        QString sqlString =
            "SELECT SUM(money) FROM transactions "
            "WHERE month = :mon AND year = :year;";

        QSqlQuery query(db);
        query.prepare(sqlString);
        query.bindValue(":mon", transaction.month);
        query.bindValue(":year", transaction.year);

        double result = -1;
        query.exec();
        if(!flushErrors(query)){
            query.first();
            result = query.value(0).toDouble();
        }
        return result;
    }

    void registerDatabase(){
        qmlRegisterSingletonType<Database>
            ("App.Database", 1, 0, "DatabaseAPI",
             [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
                Q_UNUSED(engine);
                Q_UNUSED(scriptEngine);
                return new Database();
            });
    }
}
