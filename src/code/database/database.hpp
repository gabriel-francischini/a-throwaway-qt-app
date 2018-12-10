#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QObject>
#include <QSqlDatabase>
#include <QQmlEngine>
#include "database/errors/errors.hpp"

namespace Database {

    class Database : public QObject {
        Q_OBJECT
        public:
        explicit Database(QObject *parent = nullptr) : QObject(parent) {}

        Q_INVOKABLE static
            bool addTransaction(const QString& moneyStr,
                                const QString& dateStr,
                                const QString& descStr,
                                const QString& typeStr);

        Q_INVOKABLE static
            double sumByMonthAndType(const QString& dateStr,
                                     const QString& typeStr);
        Q_INVOKABLE static
            double sumByMonth(const QString& dateStr);

        private:
        static bool dbInitialized;
        static QSqlDatabase db;

        static void startDb();
        static constexpr char const * const tableName = "transactions";
        static constexpr char const * const
            createCommand = "CREATE TABLE IF NOT EXISTS transactions ("
            "money REAL NOT NULL, description TEXT, "
            "day INTEGER NOT NULL, month INTEGER NOT NULL, "
            "year INTEGER NOT NULL, type TEXT NOT NULL);";
    };


    void registerDatabase();

}

#endif
