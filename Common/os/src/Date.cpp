
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "Date.h"

#include <sstream>

namespace SkyvoOS{

const std::string Date::monthString [13] = {"Natecember", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

Date::Date() :
    month(0), 
    day(0), 
    year(0), 
    hour(0), 
    minute(0), 
    second(0)
{

    time_t rawTime;
    struct tm *currentTime;
    time (&rawTime);
    currentTime = localtime(&rawTime);

    month = currentTime->tm_mon + 1;
    day = currentTime->tm_mday;
    year = currentTime->tm_year + 1900;
    hour = currentTime->tm_hour;
    minute = currentTime->tm_min;
    second = currentTime->tm_sec;

    #ifdef UNIT_TEST
        timeInfo = currentTime;
    #endif // UNIT_TEST

}

Date::Date(unsigned int month, unsigned int day, unsigned int year, unsigned int hour, unsigned int minute, unsigned int second):
    month(month), 
    day(day), 
    year(year), 
    hour(hour), 
    minute(minute), 
    second(second)
    #ifdef UNIT_TEST
    ,timeInfo(NULL)
    #endif
{
}

Date::~Date(){
}

unsigned int Date::getMonthNumber() const{
    return month;
}

std::string Date::getMonthString() const{
    if (month > 12){
        return monthString[0];
    }

    return monthString[month];
}

unsigned int Date::getDay() const{
    return day;
}

unsigned int Date::getYear() const{
    return year;
}

unsigned int Date::get12Hour() const{
    if (hour == 0){
        return 12;
    }
    else if (hour > 0 && hour < 13){
        return hour;
    }
    else{
        return hour-12;
    }
}

unsigned int Date::get24Hour() const{
    return hour;
}

unsigned int Date::getMinute() const{
    return minute;
}

unsigned int Date::getSecond() const{
    return second;
}

std::string Date::getAM_PM() const{
    if (hour < 12){
        return "AM";
    }
    else{
        return "PM";
    }
}

std::string Date::getDateSlashes() const{
    std::stringstream ss;
    ss << getMonthNumber() << "/" << getDay() << "/" << getYear();
    return ss.str();
}

std::string Date::getFullDateUnderscores() const
{
    std::stringstream ss;
    ss << getMonthNumber() << "_" << getDay() << "_" << getYear()  << "-" << get24Hour() << "_" << getMinute() << "_" << getSecond();
    return ss.str();
}

std::string Date::getDateFull() const{
    std::stringstream ss;
    ss << getMonthString() << " " << getDay() << ", " << getYear();
    return ss.str();
}

std::string Date::getTime12() const{
    std::stringstream ss;
    ss << get12Hour() << ":" << getMinute() << ":" << getSecond() << " " << getAM_PM();
    return ss.str();
}

std::string Date::getTime24() const{
    std::stringstream ss;
    ss << get24Hour() << ":" << getMinute() << ":" << getSecond();
    return ss.str();
}

std::string Date::getDateTimeSlashes12() const{
    std::stringstream ss;
    ss << getDateSlashes() << " " << getTime12();
    return ss.str();
}

std::string Date::getDateTimeSlashes24() const{
    std::stringstream ss;
    ss << getDateSlashes() << " " << getTime24();
    return ss.str();
}

std::string Date::getDateTimeFull12() const{
    std::stringstream ss;
    ss << getDateFull() << ". " << getTime12();
    return ss.str();
}

std::string Date::getDateTimeFull24() const{
    std::stringstream ss;
    ss << getDateFull() << ". " <<getTime24();
    return ss.str();
}

#ifdef UNIT_TEST
    const std::string * Date::getMonthNames() const{
        return monthString;
    }

    struct tm * Date::getTimeStruct() const{
        return timeInfo;
    }
#endif // UNIT_TEST

bool Date::operator==(const Date& other) const{
    if (month != other.getMonthNumber()){
        return false;
    }
    else if (day != other.getDay()){
        return false;
    }
    else if (year != other.getYear()){
        return false;
    }
    else if (hour != other.get24Hour()){
        return false;
    }
    else if (minute != other.getMinute()){
        return false;
    }
    else if (second != other.getSecond()){
        return false;
    }
    else{
        return true;
    }
}

bool Date::operator!=(const Date& other) const{
    return !((*this) == other);
}

bool Date::operator < (const Date &other) const{

    bool ret = false;
    if (year < other.year){
        ret = true;
    }
    else if (year > other.year){
        ret = false;
    }
    else{
        if (month < other.month){
            ret = true;
        }
        else if (month > other.month){
            ret = false;
        }
        else{
            if (day < other.day){
                ret = true;
            }
            else if (day > other.day){
                ret = false;
            }
            else{
                if (hour < other.hour){
                    ret = true;
                }
                else if (hour > other.hour){
                    ret = false;
                }
                else{
                    if (minute < other.minute){
                        ret = true;
                    }
                    else if (minute > other.minute){
                        ret = false;
                    }
                    else{
                        if (second < other.second){
                            ret = true;
                        }
                        else{
                            ret = false;
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool Date::operator <= (const Date &other) const{
    return (((*this) < other) || ((*this) == other));
}

bool Date::operator > (const Date &other) const{
    return (!((*this) < other)) && ((*this) != other);
}

bool Date::operator >= (const Date &other) const{
    return (((*this) > other) || ((*this) == other));
}


}
