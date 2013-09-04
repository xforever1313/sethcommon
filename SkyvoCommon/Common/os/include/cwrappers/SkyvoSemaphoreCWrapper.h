#ifndef SKYVOSEMAPHORECWRAPPER_H
#define SKYVOSEMAPHORECWRAPPER_H

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
