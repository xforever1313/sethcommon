
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Will not compile for emscripten

#include <condition_variable>
#include <ctime>
#include <mutex>

#include "SThread.h"
#include "SSemaphore.h"

namespace OS{

struct SSemaphore::SemaphoreImpl{
    SemaphoreImpl(unsigned int count) :
        m_conditionVariable(new std::condition_variable),
        m_semaphoreCount(count)
    {
    }

    std::condition_variable *m_conditionVariable;
    unsigned int m_semaphoreCount;
    std::mutex m_semaphoreCountMutex;
    std::mutex m_shutdownMutex;
	
    private:
        SemaphoreImpl(const SemaphoreImpl&);
};

SSemaphore::SSemaphore() :
    m_impl(new SemaphoreImpl(0)),
    m_isShutDown(false)
{
}

SSemaphore::SSemaphore(unsigned int initialCount) :
    m_impl(new SemaphoreImpl(initialCount)),
    m_isShutDown(false)
{
}

SSemaphore::~SSemaphore(){
    if(!isShutdown()){
        shutdown();
    }
    delete m_impl->m_conditionVariable; //Delete this here, since for some strange reason, a ~impl() destructor does not get called :|
    m_impl->m_conditionVariable = NULL;
    delete m_impl;
}

void SSemaphore::post(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    ++m_impl->m_semaphoreCount;
    m_impl->m_conditionVariable->notify_one();
    SThread::yield(); //Allow another thread to go.
}

void SSemaphore::wait(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    while (m_impl->m_semaphoreCount == 0){
        m_impl->m_conditionVariable->wait(lock);
    }
    --m_impl->m_semaphoreCount;
}

bool SSemaphore::tryWait(){
    bool ret = true;
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    if (m_impl->m_semaphoreCount == 0){
        ret = false;
    }
    else{
        --m_impl->m_semaphoreCount;
    }
    return ret;
}

bool SSemaphore::timedWait(unsigned long millisecs){
    std::cv_status timeout = std::cv_status::no_timeout;

    bool ret = true;
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    if (m_impl->m_semaphoreCount == 0){
        timeout = m_impl->m_conditionVariable->wait_for(lock, std::chrono::milliseconds(millisecs));
        if (timeout == std::cv_status::timeout){
            ret = false;
        }
    }
    else{
        --m_impl->m_semaphoreCount;
    }
    return ret;
}

void SSemaphore::shutdown(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    std::unique_lock<std::mutex> shutdownLock(m_impl->m_shutdownMutex);
    m_isShutDown = true;
    m_impl->m_semaphoreCount = -1;
    m_impl->m_conditionVariable->notify_all(); //Wake up ALL the threads
}

bool SSemaphore::isShutdown(){
    std::unique_lock<std::mutex> shutdownLock(m_impl->m_shutdownMutex);
    return m_isShutDown;
}

unsigned int SSemaphore::getSemaphoreCount(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    return m_impl->m_semaphoreCount;
}

}

#endif
