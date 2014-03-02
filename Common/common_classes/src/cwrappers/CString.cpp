
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cwrappers/CString.h"
#include "cwrappers/CStringStruct.h"

CString_t createString(const char *s){
    return CString_t(s);
}

CString_t copyString(const CString_t &s){
    return CString_t(s);
}

void appendToString(CString &s, char charToAppend){
    s.m_string += charToAppend;
}

size_t getSizeOfString(const CString_t &s){
    return s.m_string.size();
}

char getCharAtIndex(const CString_t &s, size_t index){
    return s.m_string[index];
}

const char *getCString(const CString_t &s){
    return s.m_string.c_str();
}
