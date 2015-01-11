
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Will not compile for emscripten

#include <functional>
#include <iostream>
#include <thread>

#include "SThread.h"

namespace OS{

struct SThread::SThreadImpl{

    SThreadImpl(std::function<void(void)> runner) :
        m_thread(std::thread(runner))
    {
    }

    SThreadImpl(SThreadImpl &&other) :
        m_thread(std::move(other.m_thread))
    {
    }
    
    ~SThreadImpl(){
    }

    std::thread m_thread;

};

SThread::SThread() :
    m_status(NOT_STARTED),
    m_impl(nullptr)
{
}

SThread::~SThread(){
    delete m_impl;
}

void SThread::start(){
    if (m_impl == nullptr){
        auto runFunct = std::bind(&SThread::work, this);
        m_impl = new SThreadImpl(runFunct);
        m_startCV.wait(); //Wait for the thread to start
    }
}

bool SThread::joinable() const{
    bool ret = false;
    if (m_impl != nullptr){
        ret = m_impl->m_thread.joinable();
    }
    return ret;
}

void SThread::join(){
    if ((m_impl != nullptr) && joinable()){
        m_impl->m_thread.join();
    }
}

void SThread::detach(){
    if (m_impl != nullptr){
        m_impl->m_thread.detach();
    }
}

unsigned SThread::hardware_concurrency(){
    return std::thread::hardware_concurrency();
}

void SThread::yield(){
    std::this_thread::yield();
}

void SThread::sleep(unsigned int millisecs){
    std::chrono::milliseconds duration(millisecs);
    std::this_thread::sleep_for(duration);
}

void SThread::work(){
    m_startCV.shutdown(); //Thread created, start may return
    m_status_mutex.lock();
    m_status = RUNNING;
    m_status_mutex.unlock();

    run();

    m_status_mutex.lock();
    m_status = COMPLETED;
    m_status_mutex.unlock();
}

SThread::SThreadStatus SThread::getStatus(){
    m_status_mutex.lock();
    SThread::SThreadStatus status = m_status;
    m_status_mutex.unlock();
    return status;
}

}

#endif

