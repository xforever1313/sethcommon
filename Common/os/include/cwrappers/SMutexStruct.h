
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SMUTEXSTRUCT_H_INCLUDED
#define SMUTEXSTRUCT_H_INCLUDED

#include "SMutex.h"
#include "SMutexInterface.h"

/**
* \file SMutexStruct.h
* \struct SMutex
* \brief This file is to be included in the implementation of the C wrapper, and the test
* DO NOT include this in the mutex cwrapper header.
*/

typedef struct SMutex{
    public:
        SMutex() :
            m_mutex(new OS::SMutex())
        {
        }

        virtual ~SMutex(){
            delete m_mutex;
        }

        OS::SMutexInterface *m_mutex;

    private:
        SMutex(const SMutex&); //Private copy constructor.  Should not be copied.

}SMutex_t;

#endif
