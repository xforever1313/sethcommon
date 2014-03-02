
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CSTRING_H
#define CSTRING_H

#include "cwrappers/CBoolean.h"
#include <stddef.h>

/**
* \file CString
* \brief this class provides a wrapper to std::string so it can be used in C.
* \warning Does not provide c linkage with MSVC, only with GCC/Clang.
*/
#if defined(__cplusplus) && !defined(MSVC)
extern "C" {
#endif

typedef struct CString CString_t;

/**
* \brief creates a string based on the given char array
* \return a CString structure.  This is on the stack, do not delete it.
*/
CString_t createString(const char *s);

CString_t copyString(const CString &s);

void appendToString(CString &s, char charToAppend);

/**
* \note does not include the NULL character in the size count.
*/
size_t getSizeOfString(const CString &s);

/**
* \warning there is no bounds checking.
*/
char getCharAtIndex(const CString &s, size_t index);

CBoolean areStringsEqual(const CString &s1, const CString &s2);

/**
* \brief returns the c string (char array) of the string.
* \warning this can result in undefined behavior if used incorrectly.
* If s goes out of scope, or is deleted, the pointer that is returned is now
* pointing to bad data.  If you need the cstring, but dont need CString, or its about to go
* out of scope, copy the cstring to another char array, before using it.
*/
const char *getCString(const CString_t &s);

#if defined(__cplusplus) && !defined(MSVC)
}
#endif

#endif
