
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Will not compile for emscripten

#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

#include "SThread.h"

namespace OS{

SThread::SThread() :
    m_status(NOT_STARTED)
{
}

SThread::~SThread(){
    join();
}

void SThread::start(const std::function<void()> &runFun) {

    // Do nothing if thread has started.
    if (getStatus() == SThreadStatus::NOT_STARTED) {
        m_runFunc = runFun; // Set the run function

        m_status = SThreadStatus::RUNNING; // We are now running

        auto workFunct = std::bind(&SThread::work, this);
        m_thread = std::thread(workFunct); // Start the thread
        m_startCV.wait(); //Wait for the thread to start
    }
}

bool SThread::joinable() {
    return m_thread.joinable();
}

void SThread::join(){
    if (joinable()){
        m_thread.join();
    }
}

void SThread::detach(){
    if (getStatus() == SThreadStatus::RUNNING){
        m_thread.detach();
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

    m_runFunc();

    m_status_mutex.lock();
    m_status = COMPLETED;
    m_status_mutex.unlock();
}

SThread::SThreadStatus SThread::getStatus() {
    std::lock_guard<std::mutex> lock(m_status_mutex);
    return m_status;
}

}

#endif

