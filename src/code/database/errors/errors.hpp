#ifndef DATABASE_ERRORS
#define DATABASE_ERRORS

#include <QSqlQuery>
#include <QSqlDatabase>

namespace Database {
    bool flushErrors(const QSqlQuery&);
    bool flushErrors(QSqlDatabase&);
}



#endif
