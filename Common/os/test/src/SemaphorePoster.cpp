
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Not compatible with emscripten

#include "SemaphorePoster.h"

SemaphorePoster::SemaphorePoster(OS::SSemaphore *semaphore) :
    m_semaphore(semaphore),
    m_posted(false)
{
}

SemaphorePoster::~SemaphorePoster(){
}

void SemaphorePoster::run(){
    OS::SThread::sleep(500);
    m_posted_mutex.lock();
    m_semaphore->post();
    m_posted = true;
    m_posted_mutex.unlock();
}

bool SemaphorePoster::getPosted(){
    m_posted_mutex.lock();
    bool posted = m_posted;
    m_posted_mutex.unlock();
    return posted;
}

#endif
