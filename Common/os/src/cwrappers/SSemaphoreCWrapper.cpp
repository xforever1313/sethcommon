
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include "cwrappers/CBoolean.h"
#include "cwrappers/SSemaphoreCWrapper.h"
#include "cwrappers/SSemaphoreStruct.h"

SSemaphore_t *createSemaphore(){
    return new SSemaphore_t(0);
}

SSemaphore_t *createSemaphoreWithInitialCount(unsigned int initialCount){
    return new SSemaphore_t(initialCount);
}

void deleteSemaphore(SSemaphore_t *semaphore){
    delete semaphore;
}

void postToSemaphore(SSemaphore_t *semaphore){
    semaphore->m_semaphore->post();
}

void waitOnSemaphore(SSemaphore_t *semaphore){
    semaphore->m_semaphore->wait();
}

CBoolean tryWaitOnSemaphore(SSemaphore_t *semaphore){
    CBoolean ret = FALSE;
    if(semaphore->m_semaphore->tryWait()){
        ret = TRUE;
    }
    return ret;
}

CBoolean timedWaitOnSemaphore(SSemaphore_t *semaphore, unsigned long millisecs){
    CBoolean ret = FALSE;
    if (semaphore->m_semaphore->timedWait(millisecs)){
        ret = TRUE;
    }
    return ret;
}

void shutdownSemaphore(SSemaphore_t *semaphore){
    semaphore->m_semaphore->shutdown();
}

///\return 1 if true, 0 if false
CBoolean isSemaphoreShutDown(SSemaphore_t *semaphore){
    CBoolean ret = FALSE;
    if (semaphore->m_semaphore->isShutdown()){
        ret = TRUE;
    }
    return ret;
}

#endif
