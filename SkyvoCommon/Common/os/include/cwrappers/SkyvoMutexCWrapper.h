#ifndef SKYVOMUTEXCWRAPPER_H_INCLUDED
#define SKYVOMUTEXCWRAPPER_H_INCLUDED

/**
* \file SkyvoMutexCWrapper.h
* \brief This file wrappers the C++ Skyvo Mutex so it can be called from C or Obj-C
*/

typedef struct SkyvoMutex SkyvoMutex_t;

#ifdef __cplusplus
extern "C" {
#endif

SkyvoMutex_t *createMutex();

void deleteMutex(SkyvoMutex_t *mutex);

void lockMutex(SkyvoMutex_t *mutex);

void unlockMutex(SkyvoMutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif // MUTEXCWRAPPER_H_INCLUDED
