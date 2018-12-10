#include "validators/validators.hpp"
#include <QQmlEngine>

#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <QDebug>

namespace Validators {
    bool DateValidator::isDayValid(const unsigned int& day){
        return (01 <= day) && (day <= 31);
    }

    bool DateValidator::isMonthValid(const unsigned int& month){
        return (01 <= month) && (month <= 12);
    }

    bool DateValidator::isYearValid(const unsigned int& year){
        return (1900 <= year) && (year <= 2099);
    }


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
            if(!(isYearValid(currentValue) || isDayValid(currentValue)))
                // Be sure that it will be detected as a bad number
                currentValue = 0xF << (sizeof(unsigned int) * 4);

            return static_cast<unsigned int>(currentValue);
        };

        unsigned int day = numberFieldAt(0, dateSeparatorIndexes[0]);

        if(!DateValidator::isDayValid(day))
            return false;

        // The "+ 1" is to skip the date separator and start right after it
        unsigned int month = numberFieldAt(dateSeparatorIndexes[0] + 1,
                                           dateSeparatorIndexes[1]);

        if(!DateValidator::isMonthValid(month))
            return false;

        // The "+ 1" is to skip the date separator and start right after it
        unsigned int year = numberFieldAt(dateSeparatorIndexes[1] + 1, dateLength);

        if(!DateValidator::isYearValid(year))
            return false;

        return true;
    }

    QString DateValidator::correctedDate(const QString& dateString){
        if(dateString.isEmpty())
            return "";

        std::vector<char> onlyNumbers;

        for(auto index = 0; index < dateString.length(); index++){
            unsigned char ch = dateString.at(index).toLatin1();
            // Is ch a number?
            if(static_cast<unsigned char>(ch - 0x30) <= 0x09)
                onlyNumbers.push_back(ch);
        }

        const auto vec2uint = [](const std::vector<char>& vec){
            unsigned int currentValue = 0;
            for(const unsigned char& ch : vec){
                unsigned int maybeNum = ch - 0x30;
                if(maybeNum <= 0x09){
                    currentValue *= 10;
                    currentValue += maybeNum;
                }
            }
            return currentValue;
        };
        const auto vec2qstr = [](const std::vector<char>& vec){
            QString string = "";
            for(char ch : vec){
                string.append(ch);
            }
            return string;
        };

        const auto vecTransferStart = [](std::vector<char>& from,
                                         std::vector<char>& to,
                                         unsigned int N){
            for(auto i=0; (i < N) && (from.size() > 0); i++){
                to.push_back(from[0]);
                from.erase(from.begin());
            }
        };

        std::vector<char> dayStr;
        std::vector<char> monthStr;
        std::vector<char> yearStr;

        vecTransferStart(onlyNumbers, dayStr, 2);
        vecTransferStart(onlyNumbers, monthStr, 2);
        vecTransferStart(onlyNumbers, yearStr, 4);

        // qDebug() << vec2qstr(dayStr)
        //          << vec2qstr(monthStr)
        //          << vec2qstr(yearStr);


        if(onlyNumbers.size() < 8){
            // Good start
            if(isDayValid(vec2uint(dayStr))){
                QString building = vec2qstr(dayStr);

                // So far things are good
                if(isMonthValid(vec2uint(monthStr))){
                    building += "/" + vec2qstr(monthStr);

                    // Perfect
                    if(isYearValid(vec2uint(yearStr))){
                        return building + "/" + vec2qstr(yearStr);

                        // Year may not be complete
                    } else if((yearStr.size() <= 4) && (!yearStr.empty())){
                        return building + "/" + vec2qstr(yearStr);

                        // Year may not be there
                    } else if(yearStr.empty()) {
                        return building;

                        // Force the recipe as max as possible: (19|20)[0-9]{2}
                    } else if (yearStr.size() >= 4){
                        while((yearStr[0] != '1') || (yearStr[0] != '2')){
                            yearStr.erase(yearStr.begin());
                        }
                        while(((yearStr[0] == '1')
                               ? (yearStr[1] == '9')
                               : (yearStr[1] == '0'))){
                            yearStr.erase(yearStr.begin());
                        }
                        while(yearStr.size() > 4)
                            yearStr.pop_back();

                        return building + "/" + vec2qstr(yearStr);

                        // Uh-oh, total bogus year
                    } else {
                    }

                    // Empty month
                } else if(monthStr.empty()){
                    return building;

                    // String still being typed...
                } else if(yearStr.empty()){
                    return building + "/" + vec2qstr(monthStr);

                    // Maybe month and day are misplaced
                } else if(isDayValid(vec2uint(monthStr))
                          && isMonthValid(vec2uint(dayStr))){
                    QString corrected = vec2qstr(monthStr) + "/"
                        + vec2qstr(dayStr) + "/" + vec2qstr(yearStr);

                    // Do more one pass to guarantee that this is right
                    return correctedDate(corrected);
                }
            }
        } else if(onlyNumbers.size() == 8){
        } else {
        }

        // qDebug() << "Didn't know what to do with the value.";
        return dateString;
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
