/**
 * \file DateTest.cpp
 * \brief Tests the Date class
 *
 * \author Seth Hendrick
 */

#include <CppUTest/TestHarness.h>

#define private public
#include "Date.h"

TEST_GROUP(Date){
};

/**
 * \brief Check a time before noon
 */
TEST(Date, BeforeNoonTest){
    unsigned int month = 6;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 9;
    unsigned int minute = 27;
    unsigned int second = 30;

    SkyvoOS::Date date(month, day, year, hour, minute, second);

    CHECK_EQUAL(date.getMonthNumber(), month);
    CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    CHECK_EQUAL(date.getDay(), day);
    CHECK_EQUAL(date.getYear(), year);
    CHECK_EQUAL(date.get12Hour(), hour);
    CHECK_EQUAL(date.get24Hour(), hour);
    CHECK_EQUAL(date.getMinute(), minute);
    CHECK_EQUAL(date.getSecond(), second);
    CHECK_EQUAL(date.getAM_PM(), "AM");
    CHECK_EQUAL(date.getFullDateUnderscores(), "6_13_1991-9_27_30");
    CHECK_EQUAL(date.getDateFull(), "June 13, 1991");
    CHECK_EQUAL(date.getDateSlashes(), "6/13/1991");
    CHECK_EQUAL(date.getDateTimeFull12(), "June 13, 1991. 9:27:30 AM");
    CHECK_EQUAL(date.getDateTimeFull24(), "June 13, 1991. 9:27:30");
    CHECK_EQUAL(date.getDateTimeSlashes12(), "6/13/1991 9:27:30 AM");
    CHECK_EQUAL(date.getDateTimeSlashes24(), "6/13/1991 9:27:30");
    CHECK_EQUAL(date.getTime12(), "9:27:30 AM");
    CHECK_EQUAL(date.getTime24(), "9:27:30");
}
/**
 * \brief Check a time at noon
 */
TEST(Date, AtNoonTest){
    unsigned int month = 7;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 12;
    unsigned int minute = 27;
    unsigned int second = 30;

    SkyvoOS::Date date(month, day, year, hour, minute, second);

    CHECK_EQUAL(date.getMonthNumber(), month);
    CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    CHECK_EQUAL(date.getDay(), day);
    CHECK_EQUAL(date.getYear(), year);
    CHECK_EQUAL(date.get12Hour(), hour);
    CHECK_EQUAL(date.get24Hour(), hour);
    CHECK_EQUAL(date.getMinute(), minute);
    CHECK_EQUAL(date.getSecond(), second);
    CHECK_EQUAL(date.getAM_PM(), "PM");
    CHECK_EQUAL(date.getFullDateUnderscores(), "7_13_1991-12_27_30");
    CHECK_EQUAL(date.getDateFull(), "July 13, 1991");
    CHECK_EQUAL(date.getDateSlashes(), "7/13/1991");
    CHECK_EQUAL(date.getDateTimeFull12(), "July 13, 1991. 12:27:30 PM");
    CHECK_EQUAL(date.getDateTimeFull24(), "July 13, 1991. 12:27:30");
    CHECK_EQUAL(date.getDateTimeSlashes12(), "7/13/1991 12:27:30 PM");
    CHECK_EQUAL(date.getDateTimeSlashes24(), "7/13/1991 12:27:30");
    CHECK_EQUAL(date.getTime12(), "12:27:30 PM");
    CHECK_EQUAL(date.getTime24(), "12:27:30");
}

/**
 * \brief Check a time after noon
 */
TEST(Date, AfterNoonTest){
    unsigned int month = 12;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 21;
    unsigned int minute = 27;
    unsigned int second = 30;

    SkyvoOS::Date date(month, day, year, hour, minute, second);

    CHECK_EQUAL(date.getMonthNumber(), month);
    CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    CHECK_EQUAL(date.getDay(), day);
    CHECK_EQUAL(date.getYear(), year);
    CHECK_EQUAL(date.get12Hour(), hour-12);
    CHECK_EQUAL(date.get24Hour(), hour);
    CHECK_EQUAL(date.getMinute(), minute);
    CHECK_EQUAL(date.getSecond(), second);
    CHECK_EQUAL(date.getAM_PM(), "PM");
    CHECK_EQUAL(date.getFullDateUnderscores(), "12_13_1991-21_27_30");
    CHECK_EQUAL(date.getDateFull(), "December 13, 1991");
    CHECK_EQUAL(date.getDateSlashes(), "12/13/1991");
    CHECK_EQUAL(date.getDateTimeFull12(), "December 13, 1991. 9:27:30 PM");
    CHECK_EQUAL(date.getDateTimeFull24(), "December 13, 1991. 21:27:30");
    CHECK_EQUAL(date.getDateTimeSlashes12(), "12/13/1991 9:27:30 PM");
    CHECK_EQUAL(date.getDateTimeSlashes24(), "12/13/1991 21:27:30");
    CHECK_EQUAL(date.getTime12(), "9:27:30 PM");
    CHECK_EQUAL(date.getTime24(), "21:27:30");
}

/**
 * \brief Check a time at midnight
 */
TEST(Date, AtMidNight){
    unsigned int month = 1;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 0;
    unsigned int minute = 27;
    unsigned int second = 30;

    SkyvoOS::Date date(month, day, year, hour, minute, second);

    CHECK_EQUAL(date.getMonthNumber(), month);
    CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    CHECK_EQUAL(date.getDay(), day);
    CHECK_EQUAL(date.getYear(), year);
    CHECK_EQUAL(date.get12Hour(), (unsigned int) 12);
    CHECK_EQUAL(date.get24Hour(), hour);
    CHECK_EQUAL(date.getMinute(), minute);
    CHECK_EQUAL(date.getSecond(), second);
    CHECK_EQUAL(date.getAM_PM(), "AM");
    CHECK_EQUAL(date.getFullDateUnderscores(), "1_13_1991-0_27_30");
    CHECK_EQUAL(date.getDateFull(), "January 13, 1991");
    CHECK_EQUAL(date.getDateSlashes(), "1/13/1991");
    CHECK_EQUAL(date.getDateTimeFull12(), "January 13, 1991. 12:27:30 AM");
    CHECK_EQUAL(date.getDateTimeFull24(), "January 13, 1991. 0:27:30");
    CHECK_EQUAL(date.getDateTimeSlashes12(), "1/13/1991 12:27:30 AM");
    CHECK_EQUAL(date.getDateTimeSlashes24(), "1/13/1991 0:27:30");
    CHECK_EQUAL(date.getTime12(), "12:27:30 AM");
    CHECK_EQUAL(date.getTime24(), "0:27:30");
}

/**
 * \brief Checks the error handling of a bad month.
 * \details The program should continue running, but the month is some kind of dummy name
 */
TEST(Date, BadMonthTest){
    unsigned int month1 = 0;
    unsigned int month2 = 14;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 0;
    unsigned int minute = 27;
    unsigned int second = 30;

    SkyvoOS::Date date1(month1, day, year, hour, minute, second);
    SkyvoOS::Date date2(month2, day, year, hour, minute, second);

    CHECK_EQUAL(date1.getMonthNumber(), month1);
    CHECK_EQUAL(date1.getMonthString(), date1.getMonthNames()[month1]);

    CHECK_EQUAL(date2.getMonthNumber(), month2);
    CHECK_EQUAL(date2.getMonthString(), date2.getMonthNames()[0]);
}

void lessThanTest(SkyvoOS::Date lesserDate, SkyvoOS::Date greaterDate){
    CHECK(!(lesserDate == greaterDate));
    CHECK(lesserDate != greaterDate);

    CHECK(lesserDate < greaterDate);
    CHECK(lesserDate <= greaterDate);
    CHECK(!(greaterDate < lesserDate));
    CHECK(!(greaterDate <= lesserDate));

    CHECK(!(lesserDate > greaterDate));
    CHECK(!(lesserDate >= greaterDate));
    CHECK(greaterDate > lesserDate);
    CHECK(greaterDate >= lesserDate);

}

/**
 * \brief Tests operators
 */
TEST(Date, operatorTest){
    unsigned int month1 = 4;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 0;
    unsigned int minute = 27;
    unsigned int second = 30;

    //Same Date
    SkyvoOS::Date date1(month1, day, year, hour, minute, second);
    SkyvoOS::Date date2(month1, day, year, hour, minute, second);
    CHECK(date1 == date2);
    CHECK(!(date1 != date2));

    lessThanTest(SkyvoOS::Date(0, 0, 0, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 0, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 0, 1, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 0, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(0, 1, 1, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 0, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 0, 1, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 0, 1, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 0, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 0, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 0, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 0, 1, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 1, 0, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 1, 0, 1), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
    lessThanTest(SkyvoOS::Date(1, 1, 1, 1, 1, 0), SkyvoOS::Date(1, 1, 1, 1, 1, 1));
}

/**
 * \brief Tests the default constructor, which handles the current time
 */
TEST(Date, Default){
    SkyvoOS::Date date;
    struct tm *currentTime = date.getTimeStruct();

    CHECK_EQUAL(date.getMonthNumber(), (unsigned int) currentTime->tm_mon + 1);
    CHECK_EQUAL(date.getDay(), (unsigned int) currentTime->tm_mday);
    CHECK_EQUAL(date.getYear(), (unsigned int) currentTime->tm_year + 1900);
    if (currentTime->tm_hour > 12){
        CHECK_EQUAL(date.get12Hour(), (unsigned int) currentTime->tm_hour - 12);
    }
    else if (currentTime->tm_hour == 0){
        CHECK_EQUAL(date.get12Hour(), (unsigned int) 12);
    }
    else{
        CHECK_EQUAL(date.get12Hour(), (unsigned int) currentTime->tm_hour);
    }
    CHECK_EQUAL(date.get24Hour(), (unsigned int) currentTime->tm_hour);
    CHECK_EQUAL(date.getMinute(), (unsigned int) currentTime->tm_min);
    CHECK_EQUAL(date.getSecond(), (unsigned int) currentTime->tm_sec);
}
