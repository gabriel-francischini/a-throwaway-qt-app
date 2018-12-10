#include <catch2/catch.hpp>
#include "validators/date_validator.hpp"
#include <QString>

TEST_CASE("date_validator_sane_dates", "[validators date_validator]"){
    constexpr auto& isDateValid = Validators::DateValidator::isDateValid;

    // Fair normal date
    CHECK(isDateValid("05/05/2003") == true);

    // Old but gold
    CHECK(isDateValid("05/05/1981") == true);

    // Too old, we should accept dates between 1900 and 2099
    CHECK(isDateValid("05/05/1856") == false);
    CHECK(isDateValid("05/05/2099") == true);
    CHECK(isDateValid("05/05/2100") == false);

    // Day and Month misplaced
    CHECK(isDateValid("12/31/2003") == false);

    // ISO time is (unfortunately) not supported in user input
    CHECK(isDateValid("05-05-2003") == false);

    // These are probably typos
    CHECK(isDateValid("32/05/2003") == false);
    CHECK(isDateValid("01/13/2003") == false);
    CHECK(isDateValid("00/05/2003") == false);
    CHECK(isDateValid("05/00/2003") == false);
}

TEST_CASE("date_validator_crazy_dates", "[validators date_validator]"){
    constexpr auto& isDateValid = Validators::DateValidator::isDateValid;
    CHECK(isDateValid("03/05/2003") == true);

    // No full text!
    CHECK(isDateValid("May 3rd 2003") == false);

    // Neither (almost) full text
    CHECK(isDateValid("May/3rd/2003") == false);

    CHECK(isDateValid("02/05/2003") == true);

    // No emojis inside text!
    CHECK(isDateValid("02/😎5/2003") == false);
    CHECK(isDateValid("😎2/05/2003") == false);
    CHECK(isDateValid("02/05/20😎3") == false);
    CHECK(isDateValid("02/05/2😎03") == false);

    CHECK(isDateValid("31/12/2018") == true);

    // No foreign text, unfortunately
    CHECK(isDateValid("2008年12月31日") == false);

    // No null dates
    CHECK(isDateValid("00/05/2003") == false);
    CHECK(isDateValid("03/00/2003") == false);
    CHECK(isDateValid("03/05/0000") == false);
    CHECK(isDateValid("00/00/0000") == false);
    CHECK(isDateValid("\0\0/\0\0/\0\0\0\0") == false);
    CHECK(isDateValid("\0\0\0\0\0\0\0\0\0\0") == false);

    // Also, no 99 dates or similar
    CHECK(isDateValid("99/99/9999") == false);
    CHECK(isDateValid("DD/MM/YYYY") == false);
    CHECK(isDateValid("12/34/5678") == false);
    CHECK(isDateValid("12/03/4567") == false);
    CHECK(isDateValid("12/03/2345") == false);
}

}
