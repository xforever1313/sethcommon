
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SSEMAPHORESTRUCT_H_INCLUDED
#define SSEMAPHORESTRUCT_H_INCLUDED

#include "SSemaphore.h"
#include "SSemaphoreInterface.h"

/**
* \file SSemaphoreStruct.h
* \struct SSemaphore
* \brief This file is to be included in the implementation of the C wrapper, and the test
* DO NOT include this in the semaphore cwrapper header.
*/

typedef struct SSemaphore{
    public:
        SSemaphore(unsigned int initialCount) :
            m_semaphore(new OS::SSemaphore(initialCount))
            #ifdef UNIT_TEST
            ,m_initialCount(initialCount)
            #endif
        {
        }

        virtual ~SSemaphore(){
            delete m_semaphore;
        }

        OS::SSemaphoreInterface *m_semaphore;

        #ifdef UNIT_TEST
        unsigned int m_initialCount;
        #endif

    private:
        SSemaphore(const SSemaphore&); //Private copy constructor.  Should not be copied.

}SSemaphore_t;

#endif 
