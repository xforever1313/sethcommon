#ifndef SKYVOMUTEXSTRUCT_H_INCLUDED
#define SKYVOMUTEXSTRUCT_H_INCLUDED

#include "SkyvoMutex.h"
#include "SkyvoMutexInterface.h"

/**
* \file SkyvoMutexStruct.h
* \struct SkyvoMutex
* \brief This file is to be included in the implementation of the C wrapper, and the test
* DO NOT include this in the mutex cwrapper header.
*/

typedef struct SkyvoMutex{
    public:
        SkyvoMutex() :
            m_mutex(new SkyvoOS::SkyvoMutex())
        {
        }

        virtual ~SkyvoMutex(){
            delete m_mutex;
        }

        SkyvoOS::SkyvoMutexInterface *m_mutex;

    private:
        SkyvoMutex(const SkyvoMutex&); //Private copy constructor.  Should not be copied.

}SkyvoMutex_t;

#endif // SKYVOMUTEXSTRUCT_H_INCLUDED