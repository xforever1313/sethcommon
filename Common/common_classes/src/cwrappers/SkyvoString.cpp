
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cwrappers/SkyvoString.h"
#include "cwrappers/SkyvoStringStruct.h"

SkyvoString_t createString(const char *s){
    return SkyvoString_t(s);
}

SkyvoString_t copyString(const SkyvoString_t &s){
    return SkyvoString_t(s);
}

void appendToString(SkyvoString &s, char charToAppend){
    s.m_string += charToAppend;
}

size_t getSizeOfString(const SkyvoString_t &s){
    return s.m_string.size();
}

char getCharAtIndex(const SkyvoString_t &s, size_t index){
    return s.m_string[index];
}

const char *getCString(const SkyvoString_t &s){
    return s.m_string.c_str();
}
