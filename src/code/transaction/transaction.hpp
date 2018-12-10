#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <QString>
#include <QChar>

class Transaction {
    public:
    QString desc;

    QString type;

    double money;
    void setMoney(const QString& moneyStr);

    unsigned int day;
    unsigned int month;
    unsigned int year;
    void setDate(const QString& dateStr);

};

#endif
