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
    delete m_impl;
}

void SkyvoSemaphore::post(){
    m_impl->m_semaphore->post();
}

void SkyvoSemaphore::wait(){
    m_impl->m_semaphore->wait();
}

bool SkyvoSemaphore::tryWait(){
    return m_impl->m_semaphore->try_wait();
}

bool SkyvoSemaphore::timedWait(unsigned long millisecs){
    boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(millisecs);
    return m_impl->m_semaphore->timed_wait(time);
}

}
