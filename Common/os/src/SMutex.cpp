
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //This will not linkn with emscripten

#include <mutex>
#include <thread>

#include "SMutex.h"

namespace OS{

struct SMutex::MutexImpl{
    MutexImpl() :
        m_mutex(new std::mutex())
    {
    }

    virtual ~MutexImpl(){
        delete m_mutex;
    }

    std::mutex *m_mutex;
    private:
        MutexImpl(const MutexImpl&);
};

SMutex::SMutex() :
    m_impl(new MutexImpl())
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
