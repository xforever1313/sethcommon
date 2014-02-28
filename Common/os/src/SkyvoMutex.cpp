
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //This will not linkn with emscripten

#include <mutex>
#include <thread>

#include "SkyvoMutex.h"

namespace SkyvoOS{

typedef struct mutexImpl{
    mutexImpl() :
        m_mutex(new std::mutex())
    {
    }

    virtual ~mutexImpl(){
        delete m_mutex;
    }

    std::mutex *m_mutex;
    private:
        mutexImpl(const mutexImpl&);
}mutexImpl_t;

SkyvoMutex::SkyvoMutex() :
    m_impl(new mutexImpl_t())
{
}

SkyvoMutex::~SkyvoMutex(){
    delete m_impl;
}

void SkyvoMutex::lock(){
    m_impl->m_mutex->lock();
}

void  SkyvoMutex::unlock(){
    m_impl->m_mutex->unlock();
}

}

#endif

