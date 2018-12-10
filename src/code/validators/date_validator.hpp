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
    };

    void registerDateValidator();
}

#endif
