
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //This will not linkn with emscripten

#include <mutex>
#include <thread>

#include "SMutex.h"

namespace OS{

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

SMutex::SMutex() :
    m_impl(new mutexImpl_t())
{
}

SMutex::~SMutex(){
    delete m_impl;
}

void SMutex::lock(){
    m_impl->m_mutex->lock();
}

void  SMutex::unlock(){
    m_impl->m_mutex->unlock();
}

}

#endif
