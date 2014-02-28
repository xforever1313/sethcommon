
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SKYVOSEMAPHORECWRAPPER_H
#define SKYVOSEMAPHORECWRAPPER_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
    //Undefined references are created
#endif

#include "cwrappers/SkyvoBoolean.h"

/**
* \file SkyvoSemaphoreCWrapper.h
* \brief This file wrappers the C++ Skyvo Semaphore so it can be called from C or Obj-C
*/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SkyvoSemaphore SkyvoSemaphore_t;

SkyvoSemaphore_t *createSemaphore();
SkyvoSemaphore_t *createSemaphoreWithInitialCount(unsigned int initialCount);
void deleteSemaphore(SkyvoSemaphore_t *semaphore);

void postToSemaphore(SkyvoSemaphore_t *semaphore);
void waitOnSemaphore(SkyvoSemaphore_t *semaphore);
SkyvoBoolean tryWaitOnSemaphore(SkyvoSemaphore_t *semaphore);
SkyvoBoolean timedWaitOnSemaphore(SkyvoSemaphore_t *semaphore, unsigned long millisecs);

void shutdownSemaphore(SkyvoSemaphore_t *semaphore);
SkyvoBoolean isSemaphoreShutDown(SkyvoSemaphore_t *semaphore);

#ifdef __cplusplus
}
#endif

#endif // SKYVOSEMAPHORECWRAPPER_H
