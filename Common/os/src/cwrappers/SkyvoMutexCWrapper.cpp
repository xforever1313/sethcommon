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

