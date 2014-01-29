#ifndef ASM_JS //Will not compile for emscripten

#include <functional>
#include <iostream>
#include <thread>

#include "SkyvoThread.h"

namespace SkyvoOS{

typedef struct skyvoThreadImpl{

    skyvoThreadImpl(std::function<void(void)> runner) :
        m_thread(std::thread(runner))
    {
    }

    skyvoThreadImpl(skyvoThreadImpl &&other) :
        m_thread(std::move(other.m_thread))
    {
    }
    virtual ~skyvoThreadImpl(){
    }

    std::thread m_thread;

}skyvoThreadImpl_t;

SkyvoThread::SkyvoThread() :
    m_status(NOT_STARTED),
    m_impl(NULL)
{
}

SkyvoThread::SkyvoThread(SkyvoThread &&other) :
    m_status(other.m_status),
    m_impl(other.m_impl)
{
}

SkyvoThread::~SkyvoThread(){
    delete m_impl;
}

void SkyvoThread::start(){
    if (m_impl == NULL){
        auto runFunct = std::bind(&SkyvoThread::work, this);
        m_impl = new skyvoThreadImpl(runFunct);
        m_start_semaphore.wait(); //Wait for the thread to start
    }
}

bool SkyvoThread::joinable() const{
    bool ret = false;
    if (m_impl != NULL){
        ret = m_impl->m_thread.joinable();
    }
    return ret;
}

void SkyvoThread::join(){
    if ((m_impl != NULL) && joinable()){
        m_impl->m_thread.join();
    }
}

void SkyvoThread::detach(){
    if (m_impl != NULL){
        m_impl->m_thread.detach();
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

SkyvoThread::SkyvoThreadStatus SkyvoThread::getStatus(){
    m_status_mutex.lock();
    SkyvoThread::SkyvoThreadStatus status = m_status;
    m_status_mutex.unlock();
    return status;
}

}

#endif

