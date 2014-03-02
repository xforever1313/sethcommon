
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SMUTEXCWRAPPER_H_INCLUDED
#define SMUTEXCWRAPPER_H_INCLUDED

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
    //Undefined references are created
#endif

/**
* \file SMutexCWrapper.h
* \brief This file wrappers the C++ SMutex so it can be called from C or Obj-C
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SMutex SMutex_t;

SMutex_t *createMutex();

void deleteMutex(SMutex_t *mutex);

void lockMutex(SMutex_t *mutex);

void unlockMutex(SMutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif
