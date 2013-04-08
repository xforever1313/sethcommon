/**
 * \file DateTest.cpp
 * \brief Tests the Date class
 *
 * \author Seth Hendrick
 */

#include <boost/test/unit_test.hpp>

#define private public
#include "Date.h"

/**
 * \brief Check a time before noon
 */
BOOST_AUTO_TEST_CASE(Date_BeforeNoonTest){
    unsigned int month = 6;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 9;
    unsigned int minute = 27;
    unsigned int second = 30;

    Date date(month, day, year, hour, minute, second);

    BOOST_CHECK_EQUAL(date.getMonthNumber(), month);
    BOOST_CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    BOOST_CHECK_EQUAL(date.getDay(), day);
    BOOST_CHECK_EQUAL(date.getYear(), year);
    BOOST_CHECK_EQUAL(date.get12Hour(), hour);
    BOOST_CHECK_EQUAL(date.get24Hour(), hour);
    BOOST_CHECK_EQUAL(date.getMinute(), minute);
    BOOST_CHECK_EQUAL(date.getSecond(), second);
    BOOST_CHECK_EQUAL(date.getAM_PM(), "AM");
    BOOST_CHECK_EQUAL(date.getFullDateUnderscores(), "6_13_1991-9_27_30");
    BOOST_CHECK_EQUAL(date.getDateFull(), "June 13, 1991");
    BOOST_CHECK_EQUAL(date.getDateSlashes(), "6/13/1991");
    BOOST_CHECK_EQUAL(date.getDateTimeFull12(), "June 13, 1991. 9:27:30 AM");
    BOOST_CHECK_EQUAL(date.getDateTimeFull24(), "June 13, 1991. 9:27:30");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes12(), "6/13/1991 9:27:30 AM");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes24(), "6/13/1991 9:27:30");
    BOOST_CHECK_EQUAL(date.getTime12(), "9:27:30 AM");
    BOOST_CHECK_EQUAL(date.getTime24(), "9:27:30");
}
/**
 * \brief Check a time at noon
 */
BOOST_AUTO_TEST_CASE(Date_AtNoonTest){
    unsigned int month = 7;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 12;
    unsigned int minute = 27;
    unsigned int second = 30;

    Date date(month, day, year, hour, minute, second);

    BOOST_CHECK_EQUAL(date.getMonthNumber(), month);
    BOOST_CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    BOOST_CHECK_EQUAL(date.getDay(), day);
    BOOST_CHECK_EQUAL(date.getYear(), year);
    BOOST_CHECK_EQUAL(date.get12Hour(), hour);
    BOOST_CHECK_EQUAL(date.get24Hour(), hour);
    BOOST_CHECK_EQUAL(date.getMinute(), minute);
    BOOST_CHECK_EQUAL(date.getSecond(), second);
    BOOST_CHECK_EQUAL(date.getAM_PM(), "PM");
    BOOST_CHECK_EQUAL(date.getFullDateUnderscores(), "7_13_1991-12_27_30");
    BOOST_CHECK_EQUAL(date.getDateFull(), "July 13, 1991");
    BOOST_CHECK_EQUAL(date.getDateSlashes(), "7/13/1991");
    BOOST_CHECK_EQUAL(date.getDateTimeFull12(), "July 13, 1991. 12:27:30 PM");
    BOOST_CHECK_EQUAL(date.getDateTimeFull24(), "July 13, 1991. 12:27:30");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes12(), "7/13/1991 12:27:30 PM");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes24(), "7/13/1991 12:27:30");
    BOOST_CHECK_EQUAL(date.getTime12(), "12:27:30 PM");
    BOOST_CHECK_EQUAL(date.getTime24(), "12:27:30");
}

/**
 * \brief Check a time after noon
 */
BOOST_AUTO_TEST_CASE(Date_AfterNoonTest){
    unsigned int month = 12;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 21;
    unsigned int minute = 27;
    unsigned int second = 30;

    Date date(month, day, year, hour, minute, second);

    BOOST_CHECK_EQUAL(date.getMonthNumber(), month);
    BOOST_CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    BOOST_CHECK_EQUAL(date.getDay(), day);
    BOOST_CHECK_EQUAL(date.getYear(), year);
    BOOST_CHECK_EQUAL(date.get12Hour(), hour-12);
    BOOST_CHECK_EQUAL(date.get24Hour(), hour);
    BOOST_CHECK_EQUAL(date.getMinute(), minute);
    BOOST_CHECK_EQUAL(date.getSecond(), second);
    BOOST_CHECK_EQUAL(date.getAM_PM(), "PM");
    BOOST_CHECK_EQUAL(date.getFullDateUnderscores(), "12_13_1991-21_27_30");
    BOOST_CHECK_EQUAL(date.getDateFull(), "December 13, 1991");
    BOOST_CHECK_EQUAL(date.getDateSlashes(), "12/13/1991");
    BOOST_CHECK_EQUAL(date.getDateTimeFull12(), "December 13, 1991. 9:27:30 PM");
    BOOST_CHECK_EQUAL(date.getDateTimeFull24(), "December 13, 1991. 21:27:30");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes12(), "12/13/1991 9:27:30 PM");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes24(), "12/13/1991 21:27:30");
    BOOST_CHECK_EQUAL(date.getTime12(), "9:27:30 PM");
    BOOST_CHECK_EQUAL(date.getTime24(), "21:27:30");
}

/**
 * \brief Check a time at midnight
 */
BOOST_AUTO_TEST_CASE(Date_AtMidNight){
    unsigned int month = 1;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 0;
    unsigned int minute = 27;
    unsigned int second = 30;

    Date date(month, day, year, hour, minute, second);

    BOOST_CHECK_EQUAL(date.getMonthNumber(), month);
    BOOST_CHECK_EQUAL(date.getMonthString(), date.getMonthNames()[month]);
    BOOST_CHECK_EQUAL(date.getDay(), day);
    BOOST_CHECK_EQUAL(date.getYear(), year);
    BOOST_CHECK_EQUAL(date.get12Hour(), (unsigned int) 12);
    BOOST_CHECK_EQUAL(date.get24Hour(), hour);
    BOOST_CHECK_EQUAL(date.getMinute(), minute);
    BOOST_CHECK_EQUAL(date.getSecond(), second);
    BOOST_CHECK_EQUAL(date.getAM_PM(), "AM");
    BOOST_CHECK_EQUAL(date.getFullDateUnderscores(), "1_13_1991-0_27_30");
    BOOST_CHECK_EQUAL(date.getDateFull(), "January 13, 1991");
    BOOST_CHECK_EQUAL(date.getDateSlashes(), "1/13/1991");
    BOOST_CHECK_EQUAL(date.getDateTimeFull12(), "January 13, 1991. 12:27:30 AM");
    BOOST_CHECK_EQUAL(date.getDateTimeFull24(), "January 13, 1991. 0:27:30");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes12(), "1/13/1991 12:27:30 AM");
    BOOST_CHECK_EQUAL(date.getDateTimeSlashes24(), "1/13/1991 0:27:30");
    BOOST_CHECK_EQUAL(date.getTime12(), "12:27:30 AM");
    BOOST_CHECK_EQUAL(date.getTime24(), "0:27:30");
}

/**
 * \brief Checks the error handling of a bad month.
 * \details The program should continue running, but the month is some kind of dummy name
 */
BOOST_AUTO_TEST_CASE(Date_BadMonthTest){
    unsigned int month1 = 0;
    unsigned int month2 = 14;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 0;
    unsigned int minute = 27;
    unsigned int second = 30;

    Date date1(month1, day, year, hour, minute, second);
    Date date2(month2, day, year, hour, minute, second);

    BOOST_CHECK_EQUAL(date1.getMonthNumber(), month1);
    BOOST_CHECK_EQUAL(date1.getMonthString(), date1.getMonthNames()[month1]);

    BOOST_CHECK_EQUAL(date2.getMonthNumber(), month2);
    BOOST_CHECK_EQUAL(date2.getMonthString(), date2.getMonthNames()[0]);
}

void lessThanTest(Date lesserDate, Date greaterDate){
    BOOST_CHECK(!(lesserDate == greaterDate));
    BOOST_CHECK(lesserDate != greaterDate);
   
    BOOST_CHECK(lesserDate < greaterDate);
    BOOST_CHECK(lesserDate <= greaterDate);
    BOOST_CHECK(!(greaterDate < lesserDate));
    BOOST_CHECK(!(greaterDate <= lesserDate));
    
    BOOST_CHECK(!(lesserDate > greaterDate));
    BOOST_CHECK(!(lesserDate >= greaterDate));
    BOOST_CHECK(greaterDate > lesserDate);
    BOOST_CHECK(greaterDate >= lesserDate);
    
}

/**
 * \brief Tests operators
 */
BOOST_AUTO_TEST_CASE(Date_operatorTest){
    unsigned int month1 = 4;
    unsigned int day = 13;
    unsigned int year = 1991;
    unsigned int hour = 0;
    unsigned int minute = 27;
    unsigned int second = 30;

    //Same Date
    Date date1(month1, day, year, hour, minute, second);
    Date date2(month1, day, year, hour, minute, second);
    BOOST_CHECK(date1 == date2);
    BOOST_CHECK(!(date1 != date2));

    lessThanTest(Date(0, 0, 0, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 0, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 0, 1, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 0, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(0, 1, 1, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 0, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 0, 1, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 0, 1, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 0, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 0, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 0, 1, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 0, 1, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 1, 0, 0), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 1, 0, 1), Date(1, 1, 1, 1, 1, 1));
    lessThanTest(Date(1, 1, 1, 1, 1, 0), Date(1, 1, 1, 1, 1, 1));
}

/**
 * \brief Tests the default constructor, which handles the current time
 */
BOOST_AUTO_TEST_CASE(Date_Default){
    Date date;
    struct tm *currentTime = date.getTimeStruct();

    BOOST_CHECK_EQUAL(date.getMonthNumber(), (unsigned int) currentTime->tm_mon + 1);
    BOOST_CHECK_EQUAL(date.getDay(), (unsigned int) currentTime->tm_mday);
    BOOST_CHECK_EQUAL(date.getYear(), (unsigned int) currentTime->tm_year + 1900);
    if (currentTime->tm_hour > 12){
        BOOST_CHECK_EQUAL(date.get12Hour(), (unsigned int) currentTime->tm_hour - 12);
    }
    else if (currentTime->tm_hour == 0){
        BOOST_CHECK_EQUAL(date.get12Hour(), (unsigned int) 12);
    }
    else{
        BOOST_CHECK_EQUAL(date.get12Hour(), (unsigned int) currentTime->tm_hour);
    }
    BOOST_CHECK_EQUAL(date.get24Hour(), (unsigned int) currentTime->tm_hour);
    BOOST_CHECK_EQUAL(date.getMinute(), (unsigned int) currentTime->tm_min);
    BOOST_CHECK_EQUAL(date.getSecond(), (unsigned int) currentTime->tm_sec);

}