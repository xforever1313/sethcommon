#ifndef SKYVOSTRINGSTRUCT_H_INCLUDED
#define SKYVOSTRINGSTRUCT_H_INCLUDED

/**
* \file SkvyoStringStruct
* \brief this file is to only be included for the cpp file of SkyvoString, and the SkyvoString Test.
* DO NOT INCLUDE IT ELSEWHERE
*/

#include <string>

typedef struct SkyvoString{
    SkyvoString(const char *s) :
        m_string(std::string(s))
    {

    }

    virtual ~SkyvoString(){
    }

    bool operator==(const SkyvoString &other){
        return m_string == other.m_string;
    }

    std::string m_string;
}SkyvoString_t;

#endif // SKYVOSTRINGSTRUCT_H_INCLUDED
