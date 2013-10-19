#include <condition_variable>
#include <ctime>
#include <mutex>

#include "SkyvoThread.h"
#include "SkyvoSemaphore.h"

namespace SkyvoOS{

typedef struct semaphoreImpl{
    semaphoreImpl(unsigned int count) :
        m_conditionVariable(new std::condition_variable),
        m_semaphoreCount(count)
    {
    }

    std::condition_variable *m_conditionVariable;
    unsigned int m_semaphoreCount;
    std::mutex m_semaphoreCountMutex;
    std::mutex m_shutdownMutex;
    private:
        semaphoreImpl(const semaphoreImpl&);
}semaphoreImpl_t;

SkyvoSemaphore::SkyvoSemaphore() :
    m_impl(new semaphoreImpl_t(0)),
    m_isShutDown(false)
{
}

SkyvoSemaphore::SkyvoSemaphore(unsigned int initialCount) :
    m_impl(new semaphoreImpl_t(initialCount)),
    m_isShutDown(false)
{
}

SkyvoSemaphore::~SkyvoSemaphore(){
    if(!isShutdown()){
        shutdown();
    }
    delete m_impl->m_conditionVariable; //Delete this here, since for some strange reason, a ~impl() destructor does not get called :|
    m_impl->m_conditionVariable = NULL;
    delete m_impl;
}

void SkyvoSemaphore::post(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    ++m_impl->m_semaphoreCount;
    m_impl->m_conditionVariable->notify_one();
    SkyvoThread::yield(); //Allow another thread to go.
}

void SkyvoSemaphore::wait(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    while (m_impl->m_semaphoreCount == 0){
        m_impl->m_conditionVariable->wait(lock);
    }
    --m_impl->m_semaphoreCount;
}

bool SkyvoSemaphore::tryWait(){
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

bool SkyvoSemaphore::timedWait(unsigned long millisecs){
    #ifdef _MSC_VER
    using namespace std::cv_status;  //This is here since MSVC fucking sucks.
    cv_status timeout;
    #else
    std::cv_status timeout;
    #endif
    bool ret = true;
    timeout = std::cv_status::no_timeout;
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

void SkyvoSemaphore::shutdown(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    std::unique_lock<std::mutex> shutdownLock(m_impl->m_shutdownMutex);
    m_isShutDown = true;
    m_impl->m_semaphoreCount = -1;
    m_impl->m_conditionVariable->notify_all(); //Wake up ALL the threads
}

bool SkyvoSemaphore::isShutdown(){
    std::unique_lock<std::mutex> shutdownLock(m_impl->m_shutdownMutex);
    return m_isShutDown;
}

unsigned int SkyvoSemaphore::getSemaphoreCount(){
    std::unique_lock<std::mutex> lock(m_impl->m_semaphoreCountMutex);
    return m_impl->m_semaphoreCount;
}

}
