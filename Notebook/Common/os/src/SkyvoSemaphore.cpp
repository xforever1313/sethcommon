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
    int getSemaphoreCount(){
        int count;
        m_semaphoreCountMutex.lock();
        count = m_semaphoreCount;
        m_semaphoreCountMutex.unlock();
        return count;
    }
    boost::condition_variable *m_conditionVariable;
    int m_semaphoreCount;
    boost::mutex m_semaphoreCountMutex;
    boost::mutex m_lockMutex;
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
    m_impl->m_semaphoreCountMutex.lock();
    ++m_impl->m_semaphoreCount;
    m_impl->m_semaphoreCountMutex.unlock();
    m_impl->m_conditionVariable->notify_one();
}

void SkyvoSemaphore::wait(){
    while (!isShutdown() && (m_impl->getSemaphoreCount() <= 0)){
        boost::unique_lock< boost::mutex > lock(m_impl->m_lockMutex);
        m_impl->m_conditionVariable->wait(lock);
    }

    m_impl->m_semaphoreCountMutex.lock();
    --m_impl->m_semaphoreCount;
    m_impl->m_semaphoreCountMutex.unlock();
}

bool SkyvoSemaphore::tryWait(){
    bool ret = true;
    if (!isShutdown()){
        if (m_impl->getSemaphoreCount() <= 0){
            ret = false;
        }
        else{
            m_impl->m_semaphoreCountMutex.lock();
            --m_impl->m_semaphoreCount;
            m_impl->m_semaphoreCountMutex.unlock();
        }
    }
    return ret;
}

bool SkyvoSemaphore::timedWait(unsigned long millisecs){
    bool ret = true;
    if (!isShutdown()){
        boost::unique_lock< boost::mutex > lock(m_impl->m_lockMutex);
        ret = m_impl->m_conditionVariable->timed_wait(lock, boost::posix_time::milliseconds(millisecs));
        if (ret){
            m_impl->m_semaphoreCountMutex.lock();
            --m_impl->m_semaphoreCount;
            m_impl->m_semaphoreCountMutex.unlock();
        }
    }
    return ret;
}

void SkyvoSemaphore::shutdown(){
    m_isShutdownMutex.lock();
    m_isShutDown = true;
    m_isShutdownMutex.unlock();
    m_impl->m_conditionVariable->notify_all(); //Wake up ALL the threads
}

bool SkyvoSemaphore::isShutdown(){
    bool ret;
    m_isShutdownMutex.lock();
    ret = m_isShutDown;
    m_isShutdownMutex.unlock();
    return ret;
}

int SkyvoSemaphore::getSemaphoreCount(){
    return m_impl->getSemaphoreCount();
}

}
