
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
 * \file Date.h
 * \brief Declaration of a time object
 *
 * \author Seth Hendrick
 */

#ifndef DATE_H
#define DATE_H

#include <array>
#include <string>
#include <ctime>


namespace OS{

/**
 * \class Date
 * \brief Represents a moment in time
 */
class Date{

    static const std::array<std::string, 13> monthString; ///<A string that includes all 12 months, plus a dummy month

    public:

        /**
         * \enum MONTH
         * \brief Numbers representing the 12 months, including a invalid month for 0.
         */
        enum MONTH{
            DNE = 0,
            JAN,
            FEB,
            MAR,
            APR,
            MAY,
            JUN,
            JUL,
            AUG,
            SEP,
            OCT,
            NOV,
            DEC
        };

        /**
         * \brief Default Constructor, constructs using the current time.
         */
        Date();

        /**
         * \brief Constructor using a custom time
         * \param month -the month out of 12
         * \param day - day of the month
         * \param year - the year
         * \param hour - the hour out of 24
         * \param minute - the minute
         * \param second - the second
         */
        Date(unsigned int month, unsigned int day, unsigned int year, unsigned int hour, unsigned int minute, unsigned int second);

        /**
         * \brief Destructor
         */
        ~Date();

        /**
         * \brief returns the month number
         * \return the month number
         */
        unsigned int getMonthNumber() const;

        /**
         * \brief gets the month in the form of a string
         * \return the month in string form
         */
        std::string getMonthString() const;

        /**
         * \brief gets the day number
         * \return the day of month number
         */
        unsigned int getDay() const;

        /**
         * \brief gets the year number
         * \brief the year number
         */
        unsigned int getYear() const;

        /**
         * \brief assuming 24 hour days, returns non-military time
         * \return the 12 hour time
         */
        unsigned int get12Hour() const;

        /**
         * \brief returns the hour number
         * \return the hour
         */
        unsigned int get24Hour() const;

        /**
         * \brief returns the minute
         * \return the minute
         */
        unsigned int getMinute() const;

        /**
         * \brief returns the second
         * \return the second
         */
        unsigned int getSecond() const;

        /**
         * \brief Assuming 24 hours in a day, it returns AM if hour is between 0-12, or PM if 13-23
         * \return AM if morning, PM if afternoon
         */
        std::string getAM_PM() const;

        /**
         * \brief Returns the date in the from of mm/dd/yyyy
         * \return date in mm/dd/yyyy form
         */
        std::string getDateSlashes() const;

         /**
         * \brief Returns the date in the from of mm_dd_yyyy
         * \return date in mm_dd_yyyy form
         */
        std::string getFullDateUnderscores() const;

        /**
         * \brief Returns the date with the month name
         * \return The date in the form of "Month day, year"
         */
        std::string getDateFull() const;

        /**
         * \brief Returns the time in 12 hour form (Includes AM/PM)
         * \return The time in HH:MM:SS AM/PM
         */
        std::string getTime12() const;

        /**
         * \brief Returns the time in 24 hour form (No AM or PM)
         * \return The time in HH:MM:SS
         */
        std::string getTime24() const;

        /**
         * \brief Returns the date with slashes, and the time in 12 hours
         * \return The date and time in mm/dd/yyyy HH:MM:SS AM/PM
         */
        std::string getDateTimeSlashes12() const;

        /**
         * \brief Returns the date with slashes, and the time in 24 hours
         * \return The date and time in mm/dd/yyyy HH:MM:SS
         */
        std::string getDateTimeSlashes24() const;

        /**
         * \brief Returns the full date with name and the time in 12 hours
         * \return The date and time in Month day, year.  HH:MM:SS AM/PM
         */
        std::string getDateTimeFull12() const;

        /**
         * \brief Returns the full date with name and the time in 24 hours
         * \return The date and time in Month day, year.  HH:MM:SS AM/PM
         */
        std::string getDateTimeFull24() const;

        /**
		 * \brief operator to see if two Dates are equal
		 * \param Another Date object to compare
		 * \return true if the two Dates are equal
		 */
        bool operator == (const Date &other) const;

        bool operator != (const Date &other) const;

        bool operator < (const Date &other) const;

        bool operator <= (const Date &other) const;

        bool operator > (const Date &other) const;

        bool operator >= (const Date &other) const;

        #ifdef UNIT_TEST
            /**
             * \brief Returns th e array of month names
             * \brief The month names
             */
             const std::string  *getMonthNames() const;

             /**
              * \brief Returns the current time's structure
              * \return the time structure
              */
              struct tm * getTimeStruct() const;

        #endif

    private:

        unsigned int month; ///<The current Month
        unsigned int day;   ///<The current day
        unsigned int year;  ///<The current year
        unsigned int hour;  ///<The current hour
        unsigned int minute;///<The current minute
        unsigned int second;///<The current second

        #ifdef UNIT_TEST
            struct tm *timeInfo; ///<A time structure with the current time
        #endif
};


}
#endif // DATE_H
