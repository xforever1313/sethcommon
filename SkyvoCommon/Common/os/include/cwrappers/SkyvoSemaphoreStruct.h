#ifndef SKYVOSEMAPHORESTRUCT_H_INCLUDED
#define SKYVOSEMAPHORESTRUCT_H_INCLUDED

#include "SkyvoSemaphore.h"
#include "SkyvoSemaphoreInterface.h"

/**
* \file SkyvoSemaphoreStruct.h
* \struct SkyvoSemaphore
* \brief This file is to be included in the implementation of the C wrapper, and the test
* DO NOT include this in the semaphore cwrapper header.
*/

typedef struct SkyvoSemaphore{
    public:
        SkyvoSemaphore(unsigned int initialCount) :
            m_semaphore(new SkyvoOS::SkyvoSemaphore(initialCount))
            #ifdef UNIT_TEST
            ,m_initialCount(initialCount)
            #endif
        {
        }

        virtual ~SkyvoSemaphore(){
            delete m_semaphore;
        }

        SkyvoOS::SkyvoSemaphoreInterface *m_semaphore;

        #ifdef UNIT_TEST
        unsigned int m_initialCount;
        #endif

    private:
        SkyvoSemaphore(const SkyvoSemaphore&); //Private copy constructor.  Should not be copied.

}SkyvoSemaphore_t;

#endif // SKYVOSEMAPHORESTRUCT_H_INCLUDED
