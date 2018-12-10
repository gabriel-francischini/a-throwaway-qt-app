#include "validators/validators.hpp"
#include <QQmlEngine>

#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <QDebug>

namespace Validators {
    bool DateValidator::isDateValid(const QString& dateString){
        // We don't store the null termination, \0
        // see: https://stackoverflow.com/questions/1392200
        constexpr unsigned long int dateLength
            = (sizeof("DD/MM/YYYY") - 1)/sizeof('D');

        // If it's well formated, then it have EXACTLY the size of "DD/MM/YYYY"
        if(dateString.length() != dateLength)
            return false;

        std::array<char, dateLength> date;

        // Date separator
        constexpr char dateSeparator = '/';
        constexpr unsigned int dateSeparatorIndexes[] = {2, 5};

        // Make a local (stack) copy of the string.
        // If it's well formated then it's in "DD/MM/YYYY" format, i.e. size 10
        for(unsigned long int index = 0; index < dateLength; index++)
            date[index] = dateString.at(index).toLatin1();

        // Checks if separators are in the right place
        bool areSeparatorsOk = true;
        for(const unsigned int& index : dateSeparatorIndexes)
            areSeparatorsOk = (date[index] == dateSeparator) && areSeparatorsOk;

        if(!areSeparatorsOk)
            return false;

        // Function for retrieving date fields (converts char[] into numbers)
        const auto numberFieldAt = [&date](const unsigned int& start,
                                           const unsigned int& end){
            unsigned long int currentValue = 0;
            for(unsigned int index = start;index < end; index++){
                currentValue *= 10;

                // ASCII numbers goes from 0x30 '0' to 0x39 '9'
                currentValue +=
                    static_cast<unsigned int>(date[index]) - 0x30;
            }

            // Uh-oh, this probably isn't a number
            if(currentValue > 9999) // Max year is 9999
                // Be sure that it will be detected as a bad number
                currentValue = 0xF << (sizeof(unsigned int) * 4);

            return static_cast<unsigned int>(currentValue);
        };

        unsigned int day = numberFieldAt(0, dateSeparatorIndexes[0]);

        if(!((01 <= day) && (day <= 31)))
            return false; // Day less than zero or bigger than 31

        // The "+ 1" is to skip the date separator and start right after it
        unsigned int month = numberFieldAt(dateSeparatorIndexes[0] + 1,
                                           dateSeparatorIndexes[1]);

        if(!((01 <= month) && (month <= 12)))
            return false; // Month less than zero or bigger than 12

        // The "+ 1" is to skip the date separator and start right after it
        unsigned int year = numberFieldAt(dateSeparatorIndexes[1] + 1, dateLength);

        if(!((1900 <= year) && (year <= 2099)))
            return false; // Year less than 0 or bigger than 2100

        qDebug() << "dateString is: '" << dateString << "'";
        return true;
    }

    void registerDateValidator(){
        qmlRegisterSingletonType<DateValidator>
            ("App.Validators", 1, 0, "DateValidator",
             [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
                Q_UNUSED(engine);
                Q_UNUSED(scriptEngine);
                return new DateValidator();
            });
    }
}
