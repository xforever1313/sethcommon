
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include "cwrappers/SkyvoBoolean.h"
#include "cwrappers/SkyvoSemaphoreCWrapper.h"
#include "cwrappers/SkyvoSemaphoreStruct.h"

SkyvoSemaphore_t *createSemaphore(){
    return new SkyvoSemaphore_t(0);
}

SkyvoSemaphore_t *createSemaphoreWithInitialCount(unsigned int initialCount){
    return new SkyvoSemaphore_t(initialCount);
}

void deleteSemaphore(SkyvoSemaphore_t *semaphore){
    delete semaphore;
}

void postToSemaphore(SkyvoSemaphore_t *semaphore){
    semaphore->m_semaphore->post();
}

void waitOnSemaphore(SkyvoSemaphore_t *semaphore){
    semaphore->m_semaphore->wait();
}

SkyvoBoolean tryWaitOnSemaphore(SkyvoSemaphore_t *semaphore){
    SkyvoBoolean ret = FALSE;
    if(semaphore->m_semaphore->tryWait()){
        ret = TRUE;
    }
    return ret;
}

SkyvoBoolean timedWaitOnSemaphore(SkyvoSemaphore_t *semaphore, unsigned long millisecs){
    SkyvoBoolean ret = FALSE;
    if (semaphore->m_semaphore->timedWait(millisecs)){
        ret = TRUE;
    }
    return ret;
}

void shutdownSemaphore(SkyvoSemaphore_t *semaphore){
    semaphore->m_semaphore->shutdown();
}

///\return 1 if true, 0 if false
SkyvoBoolean isSemaphoreShutDown(SkyvoSemaphore_t *semaphore){
    SkyvoBoolean ret = FALSE;
    if (semaphore->m_semaphore->isShutdown()){
        ret = TRUE;
    }
    return ret;
}

#endif

