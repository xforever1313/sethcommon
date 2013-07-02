#include <thread>

#include "SkyvoThread.h"

namespace SkyvoOS{

typedef struct skyvoThreadImpl{
    skyvoThreadImpl(SkyvoThread *thread) :
        m_skyvoThread(thread)
    {
    }

    virtual ~skyvoThreadImpl(){
    }
    void operator()(){
        m_skyvoThread->work();
    }
    SkyvoThread *m_skyvoThread;
}skyvoThreadImpl_t;

SkyvoThread::SkyvoThread() :
    m_status(NOT_STARTED),
    m_impl(NULL)
{
}

SkyvoThread::SkyvoThread(SkyvoThread &&other) :
    m_status(other.m_status),
    m_thread(std::move(other.m_thread)),
    m_impl(other.m_impl)
{
}

SkyvoThread::~SkyvoThread(){
    delete m_impl;
}

void SkyvoThread::start(){
    if (m_impl == NULL){
        m_impl = new skyvoThreadImpl(this);
        std::thread thread (std::ref((*m_impl)));
        m_thread = std::move(thread);
        m_start_semaphore.wait(); //Wait for the thread to start
    }
}

bool SkyvoThread::joinable() const{
    bool ret = false;
    if (m_impl != NULL){
        ret = m_thread.joinable();
    }
    return ret;
}

void SkyvoThread::join(){
    if ((m_impl != NULL) && joinable()){
        m_thread.join();
    }
}

void SkyvoThread::detach(){
    if (m_impl != NULL){
        m_thread.detach();
    }
}

unsigned SkyvoThread::hardware_concurrency(){
    return std::thread::hardware_concurrency();
}

void SkyvoThread::yield(){
    std::this_thread::yield();
}

void SkyvoThread::sleep(unsigned int millisecs){
    std::chrono::milliseconds duration(millisecs);
    std::this_thread::sleep_for(duration);
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
