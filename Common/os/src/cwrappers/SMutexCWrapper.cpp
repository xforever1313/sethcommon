
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include "cwrappers/SMutexCWrapper.h"
#include "cwrappers/SMutexStruct.h"

SMutex_t *createMutex(){
    return new SMutex();
}

void deleteMutex(SMutex_t *mutex){
    delete mutex;
}

void lockMutex(SMutex_t *mutex){
    mutex->m_mutex->lock();
}

void unlockMutex(SMutex_t *mutex){
    mutex->m_mutex->unlock();
}

#endif
