#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ctime>
#include <iostream>
#include "SkyvoSemaphore.h"

namespace SkyvoOS{

typedef struct semaphoreImpl{
    semaphoreImpl(unsigned int count) :
        m_conditionVariable(new boost::condition_variable),
        m_semaphoreCount(count)
    {
    }

    boost::condition_variable *m_conditionVariable;
    unsigned int m_semaphoreCount;
    boost::mutex m_semaphoreCountMutex;
    boost::mutex m_shutdownMutex;
}semaphoreImpl_t;

SkyvoSemaphore::SkyvoSemaphore() :
    m_impl(new semaphoreImpl_t(0)),
    m_isShutDown(false)
{
}

SkyvoSemaphore::SkyvoSemaphore(unsigned int initialCount) :
    m_impl(new semaphoreImpl_t(initialCount))
{
}

SkyvoSemaphore::~SkyvoSemaphore(){
    shutdown();
    delete m_impl->m_conditionVariable; //Delete this here, since for some strange reason, a ~impl() destructor does not get called :|
    m_impl->m_conditionVariable = NULL;
    delete m_impl;
}

void SkyvoSemaphore::post(){
    //if (!isShutdown()){
        boost::unique_lock<boost::mutex> lock(m_impl->m_semaphoreCountMutex);
        ++m_impl->m_semaphoreCount;
        m_impl->m_conditionVariable->notify_one();
    //}
}

void SkyvoSemaphore::wait(){
    boost::unique_lock<boost::mutex> lock(m_impl->m_semaphoreCountMutex);
    while ((m_impl->m_semaphoreCount == 0)){
        m_impl->m_conditionVariable->wait(lock);
    }
    --m_impl->m_semaphoreCount;
}

bool SkyvoSemaphore::tryWait(){
    bool ret = true;
    boost::unique_lock<boost::mutex> lock(m_impl->m_semaphoreCountMutex);
    if (m_impl->m_semaphoreCount == 0){
        ret = false;
    }
    else{
        --m_impl->m_semaphoreCount;
    }
    return ret;
}

bool SkyvoSemaphore::timedWait(unsigned long millisecs){
    bool ret = true;
    boost::unique_lock<boost::mutex> lock(m_impl->m_semaphoreCountMutex);
    if (m_impl->m_semaphoreCount == 0){
        ret = m_impl->m_conditionVariable->timed_wait(lock, boost::posix_time::milliseconds(millisecs));
        if (ret){
            --m_impl->m_semaphoreCount;
        }
    }
    return ret;
}

void SkyvoSemaphore::shutdown(){
    boost::unique_lock<boost::mutex> lock(m_impl->m_semaphoreCountMutex);
    boost::unique_lock<boost::mutex> shutdownLock(m_impl->m_shutdownMutex);
    m_isShutDown = true;
    m_impl->m_semaphoreCount = -1;
    m_impl->m_conditionVariable->notify_all(); //Wake up ALL the threads
}

bool SkyvoSemaphore::isShutdown(){
    boost::unique_lock<boost::mutex> shutdownLock(m_impl->m_shutdownMutex);
    return m_isShutDown;
}

}
