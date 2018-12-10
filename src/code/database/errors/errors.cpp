#include "database/errors/errors.hpp"

#include <QSqlError>
#include <QMap>
#include <QStringList>
#include <QVariant>
#include <QDebug>

#define LOC(x) ((int *) &x)

//! Prints errors that happened in the database
bool Database::flushErrors(QSqlDatabase &db){
	QSqlError error = db.lastError();
	if(error.isValid()){
		qDebug() << "[SQLDB ERROR]"
                 << error.nativeErrorCode()
                 << " - "
                 << error.text();
		return true;
	} else {
		return false;
	}
}

//! Prints errors that happened inside the query
bool Database::flushErrors(const QSqlQuery &query){
	QSqlError error = query.lastError();

    QString str = query.lastQuery();

    // Seeks arguments which were bind to the query
    QMap<QString, QVariant> arguments(query.boundValues());

    // Seeks arguments names
    QStringList keys = arguments.keys();

    // Replaces longer names first
    std::sort(keys.begin(), keys.end(),
              [](const QString& a, const QString& b) -> bool {
                  return a.length() > b.length();
              });

    // Replaces every argument (?, :variable, etc.) by it's "real" value
    for(QString& key : keys)
        str.replace(key, arguments[key].toString());


    if(error.isValid()){
		qDebug() << "[OBJADDR " << LOC(error) << "] - "
                 << "[SQLQUERY FATALQUERY]" << str;
        qDebug() << "[OBJADDR " << LOC(error) << "] - "
                 << "[SQLQUERY ERROR]" << error.nativeErrorCode()
                 << " - " << error.text();
        return true;
    } else {
        qDebug() << "[OBJADDR " << LOC(query) << "] - "
                 << "[SQLQUERY QUERY]" << str;
        return false;
    }
}


#undef LOC
