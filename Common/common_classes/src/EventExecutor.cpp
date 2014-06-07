
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Will not link do to threading

#include <memory>
#include <queue>

#include "EventExecutor.h"
#include "EventInterface.h"

namespace Common{

#ifdef UNIT_TEST
bool EventExecutor::startRightAway = true;
#endif

EventExecutor::EventExecutor() :
    m_isRunning(true)
{
    #ifdef UNIT_TEST
    if(EventExecutor::startRightAway){
    #endif
    start();
    #ifdef UNIT_TEST
    }
    #endif
}

EventExecutor::~EventExecutor(){
    m_isRunningMutex.lock();
    m_isRunning = false;
    m_eventSemaphore.shutdown(); //Allows run thread to exit
    m_isRunningMutex.unlock();
    join(); //wait for thread to exit
    //Run all unexecuted events
    while (!m_eventList.empty()){  //No need to put m_eventList in mutexes, as the run thread has exited.
        executeEvent();
    }
}

void EventExecutor::addEvent(const std::shared_ptr<EventInterface> &newEvent){
    m_eventListMutex.lock();
    m_eventList.push(newEvent);
    m_eventSemaphore.post();
    m_eventListMutex.unlock();
}

void EventExecutor::executeEvent(){
    std::shared_ptr<EventInterface> eventToRun = nullptr;
    m_eventListMutex.lock();
    if (!m_eventList.empty()){
        eventToRun = m_eventList.front(); //Copy the pointer to a temp var
        m_eventList.pop();
    }
    m_eventListMutex.unlock();
    if (eventToRun != nullptr){
        eventToRun->execute();
    }
}

void EventExecutor::run(){
    while (isRunning()){
        m_eventSemaphore.wait();
        executeEvent();
    }
}

bool EventExecutor::isRunning(){
    m_isRunningMutex.lock();
    bool ret = m_isRunning;
    m_isRunningMutex.unlock();
    return ret;
}

}

#endif
