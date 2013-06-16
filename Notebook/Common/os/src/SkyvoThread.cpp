#include <boost/thread.hpp>

#include <functional>

#include "SkyvoThread.h"

namespace SkyvoOS{

typedef struct skyvoThreadImpl{
    skyvoThreadImpl(std::function<void(void)> func) :
        m_thread(new boost::thread(func))
    {
    }
    skyvoThreadImpl(skyvoThreadImpl &&other) noexcept:
        m_thread(other.m_thread)
    {
    }
    ~skyvoThreadImpl(){
        m_thread->join();
        delete m_thread;
    }

    boost::thread *m_thread;
}skyvoThreadImpl_t;

SkyvoThread::SkyvoThread():
    m_status(NOT_STARTED),
    m_impl(NULL)
{
}

SkyvoThread::SkyvoThread(SkyvoThread &&other) noexcept :
    m_status(other.m_status),
    m_impl(other.m_impl)
{
}

SkyvoThread::~SkyvoThread(){
    delete m_impl;
}

void SkyvoThread::start(){
    if (m_impl == NULL){
        auto runFunc = std::bind (&SkyvoThread::work, this);
        m_impl = new skyvoThreadImpl_t(runFunc);
        m_start_semaphore.wait(); //Wait for the thread to start
    }
}

bool SkyvoThread::joinable() const{
    bool ret = false;
    if (m_impl != NULL){
        ret = m_impl->m_thread->joinable();
    }
    return ret;
}

void SkyvoThread::join(){
    if (m_impl != NULL){
        m_impl->m_thread->join();
    }
}

void SkyvoThread::detach(){
    if (m_impl != NULL){
        m_impl->m_thread->detach();
    }
}

unsigned SkyvoThread::hardware_concurrency(){
    return boost::thread::hardware_concurrency();
}

void SkyvoThread::interrupt(){
    if (m_impl != NULL){
        m_impl->m_thread->interrupt();
    }
}

bool SkyvoThread::interruption_requested() const{
    bool ret = false;
    if (m_impl != NULL){
        ret = m_impl->m_thread->interruption_requested();
    }
    return ret;
}

void SkyvoThread::sleep(unsigned int millisecs){
    boost::this_thread::sleep(boost::posix_time::milliseconds(millisecs));
}

void SkyvoThread::work(){
    m_start_semaphore.post(); //Thread created, start may return
    m_status_mutex.lock();
    m_status = RUNNING;
    m_status_mutex.unlock();

    run();

    m_status_mutex.lock();
    m_status = COMPLETED;
    m_status_mutex.unlock();
}

SkyvoThread::SkyvoThreadStatus SkyvoThread::getStatus() const{
    m_status_mutex.lock();
    SkyvoThread::SkyvoThreadStatus status = m_status;
    m_status_mutex.unlock();
    return status;
}

}
