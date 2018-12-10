#include "transaction/transaction.hpp"

unsigned int char2num(const unsigned char& ch){
    if((0x30 <= ch) && (ch <= 0x39))
        return (ch - 0x30);
    else
        return 0xFF;
}

unsigned int char2num(const QChar& ch){
    return char2num(ch.toLatin1());
}


void Transaction::setMoney(const QString& moneyStr){
    bool afterSeparator = false;
    double wholePart = 0;
    double fractionalPart = 0;
    for(const auto& i : moneyStr){
        unsigned char ch = i.toLatin1();
        if(ch == ',')
            afterSeparator = true;

        unsigned int num = char2num(ch);
        if(num > 9)
            continue;

        if(!afterSeparator){
            wholePart *= 10;
            wholePart += num;
        } else {
            fractionalPart *= 10;
            fractionalPart += num;
        }

    }
        while(fractionalPart > 1)
            fractionalPart /= 10;

        this->money = wholePart + fractionalPart;
}

void Transaction::setDate(const QString& dateStr){
    this->day = char2num(dateStr.at(0)) * 10 + char2num(dateStr.at(1));
    this->month = char2num(dateStr.at(3)) * 10 + char2num(dateStr.at(4));
    this->year = 0
        + char2num(dateStr.at(6)) * 1000
        + char2num(dateStr.at(7)) * 100
        + char2num(dateStr.at(8)) * 10
        + char2num(dateStr.at(9)) * 1;
}
