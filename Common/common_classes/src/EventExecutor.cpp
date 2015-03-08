
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Will not link do to threading

#include <memory>
#include <mutex>
#include <queue>

#include "EventExecutor.h"
#include "EventInterface.h"
#include "SMutex.h"

namespace Common{

EventExecutor::EventExecutor() :
    m_isRunning(false)
{
}

EventExecutor::~EventExecutor(){
    bool wasRunning;
    {
        std::lock_guard<OS::SMutex> lock(m_isRunningMutex);

        // See if we were running in the first place.
        wasRunning = m_isRunning;

        // Exit run loop.
        m_isRunning = false;

        // Allows run thread to exit.
        m_eventSemaphore.shutdown();

    } // Unlock Mutex

    join(); //wait for thread to exit

    //Run all unexecuted events if only start was called

    if (wasRunning) {
        // No need to put m_eventList in mutexes,
        // as the run thread has exited.
        while (!m_eventList.empty()){
            executeEvent();
        }
    }
}

void EventExecutor::addEvent(const std::shared_ptr<EventInterface> &newEvent){
    std::lock_guard<OS::SMutex> lock(m_eventListMutex);
    m_eventList.push(newEvent);
    m_eventSemaphore.post();
}

void EventExecutor::executeEvent(){
    std::shared_ptr<EventInterface> eventToRun = nullptr;

    {
        std::lock_guard<OS::SMutex> lock(m_eventListMutex);
        if (!m_eventList.empty()){
            // Copy the pointer to a temp var
            eventToRun = m_eventList.front();
            m_eventList.pop();
        }
    } // Mutex unlocks

    if (eventToRun != nullptr){
        eventToRun->execute();
    }
}

void EventExecutor::startExecutor() {
    m_isRunning = true;
    OS::SThread::start();
}

void EventExecutor::run(){
    while (isRunning()){
        m_eventSemaphore.wait();
        executeEvent();
    }
}

bool EventExecutor::isRunning(){
    std::lock_guard<OS::SMutex> lock(m_isRunningMutex);
    return m_isRunning;
}

}

#endif
