
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CSTRINGSTRUCT_H_INCLUDED
#define CSTRINGSTRUCT_H_INCLUDED

/**
* \file SkvyoStringStruct
* \brief this file is to only be included for the cpp file of CString, and the CString Test.
* DO NOT INCLUDE IT ELSEWHERE
*/

#include <string>

typedef struct CString{
    CString(const char *s) :
        m_string(std::string(s))
    {

    }

    virtual ~CString(){
    }

    bool operator==(const CString &other){
        return m_string == other.m_string;
    }

    std::string m_string;
}CString_t;

#endif 
