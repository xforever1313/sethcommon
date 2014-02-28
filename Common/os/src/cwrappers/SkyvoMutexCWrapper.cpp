
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include "cwrappers/SkyvoMutexCWrapper.h"
#include "cwrappers/SkyvoMutexStruct.h"

SkyvoMutex_t *createMutex(){
    return new SkyvoMutex();
}

void deleteMutex(SkyvoMutex_t *mutex){
    delete mutex;
}

void lockMutex(SkyvoMutex_t *mutex){
    mutex->m_mutex->lock();
}

void unlockMutex(SkyvoMutex_t *mutex){
    mutex->m_mutex->unlock();
}

#endif

