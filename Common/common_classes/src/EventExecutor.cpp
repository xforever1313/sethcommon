#ifndef ASM_JS //Will not link do to threading

#include <cstddef> //Needed for NULL
#include <list>
#include <SkyvoThread.h>

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

void EventExecutor::addEvent(EventInterface *newEvent){
    m_eventListMutex.lock();
    m_eventList.push_back(newEvent);
    m_eventSemaphore.post();
    m_eventListMutex.unlock();
}

void EventExecutor::executeEvent(){
    EventInterface *eventToRun = NULL;
    m_eventListMutex.lock();
    if (!m_eventList.empty()){
        eventToRun = m_eventList.front(); //Copy the pointer to a temp var
        std::list<EventInterface*>::iterator it = m_eventList.begin();
        (*it) = NULL; //Set the pointer to NULL so we can remove from the list
        m_eventList.pop_front();
    }
    m_eventListMutex.unlock();
    if (eventToRun != NULL){
        eventToRun->execute();
        delete eventToRun;
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

