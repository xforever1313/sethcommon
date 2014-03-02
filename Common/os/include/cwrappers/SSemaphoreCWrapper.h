
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SSEMAPHORECWRAPPER_H
#define SSEMAPHORECWRAPPER_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
    //Undefined references are created
#endif

#include "cwrappers/CBoolean.h"

/**
* \file SSemaphoreCWrapper.h
* \brief This file wrappers the C++ SSemaphore so it can be called from C or Obj-C
*/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SSemaphore SSemaphore_t;

SSemaphore_t *createSemaphore();
SSemaphore_t *createSemaphoreWithInitialCount(unsigned int initialCount);
void deleteSemaphore(SSemaphore_t *semaphore);

void postToSemaphore(SSemaphore_t *semaphore);
void waitOnSemaphore(SSemaphore_t *semaphore);
CBoolean tryWaitOnSemaphore(SSemaphore_t *semaphore);
CBoolean timedWaitOnSemaphore(SSemaphore_t *semaphore, unsigned long millisecs);

void shutdownSemaphore(SSemaphore_t *semaphore);
CBoolean isSemaphoreShutDown(SSemaphore_t *semaphore);

#ifdef __cplusplus
}
#endif

#endif 
