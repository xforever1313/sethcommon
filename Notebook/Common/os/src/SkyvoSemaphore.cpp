#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <ctime>
#include "SkyvoSemaphore.h"

namespace SkyvoOS{

typedef struct impl{
    impl(unsigned int count) :
        m_semaphore(new boost::interprocess::interprocess_semaphore(count))
    {
    }
    ~impl(){
        //Contrary to what code coverage says, this seems to be executed.
        //If you set m_impl->m_semaphore to NULL before deleting m_impl, a memory leak will happen, therefore, the semaphore
        //Is being deleted.
        delete m_semaphore;
    }

    boost::interprocess::interprocess_semaphore *m_semaphore;
}impl;

SkyvoSemaphore::SkyvoSemaphore() :
    m_impl(new impl(0))
{
}

SkyvoSemaphore::SkyvoSemaphore(unsigned int initialCount) :
    m_impl(new impl(initialCount))
{
}

SkyvoSemaphore::~SkyvoSemaphore(){
    m_isShutdownMutex.lock();
    delete m_impl;
    m_impl = NULL;
    m_isShutdownMutex.unlock();
}

void SkyvoSemaphore::post(){
    if (!isShutDown()){
        m_impl->m_semaphore->post();
    }
}

void SkyvoSemaphore::wait(){
    if (!isShutDown()){
        m_impl->m_semaphore->wait();
    }
}

bool SkyvoSemaphore::tryWait(){
    bool ret = true;
    if (!isShutDown()){
        ret = m_impl->m_semaphore->try_wait();
    }
    return ret;
}

bool SkyvoSemaphore::timedWait(unsigned long millisecs){
    bool ret = true;
    if (!isShutDown()){
        boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(millisecs);
        ret = m_impl->m_semaphore->timed_wait(time);
    }
    return ret;
}

void SkyvoSemaphore::shutDown(){
    m_isShutdownMutex.lock();
    delete m_impl;
    m_impl = NULL;
    m_isShutdownMutex.unlock();
}

bool SkyvoSemaphore::isShutDown(){
    bool ret;
    m_isShutdownMutex.lock();
    ret = (m_impl == NULL);
    m_isShutdownMutex.unlock();
    return ret;
}

}
