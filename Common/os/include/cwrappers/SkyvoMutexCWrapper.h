#ifndef SKYVOMUTEXCWRAPPER_H_INCLUDED
#define SKYVOMUTEXCWRAPPER_H_INCLUDED

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
    //Undefined references are created
#endif

/**
* \file SkyvoMutexCWrapper.h
* \brief This file wrappers the C++ Skyvo Mutex so it can be called from C or Obj-C
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SkyvoMutex SkyvoMutex_t;

SkyvoMutex_t *createMutex();

void deleteMutex(SkyvoMutex_t *mutex);

void lockMutex(SkyvoMutex_t *mutex);

void unlockMutex(SkyvoMutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif // MUTEXCWRAPPER_H_INCLUDED
