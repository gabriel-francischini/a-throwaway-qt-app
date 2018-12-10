#ifndef DATE_VALIDATOR_HPP
#define DATE_VALIDATOR_HPP

#include <QObject>
#include <QString>

namespace Validators {

    class DateValidator : public QObject {
        Q_OBJECT
        public:
        explicit DateValidator(QObject *parent = nullptr)
            : QObject(parent) {}

        Q_INVOKABLE static bool isDateValid(const QString& dateString);
        Q_INVOKABLE static QString correctedDate(const QString& dateString);

        private:
        static bool isDayValid(const unsigned int& day);
        static bool isMonthValid(const unsigned int& month);
        static bool isYearValid(const unsigned int& year);
    };

    void registerDateValidator();
}

#endif
