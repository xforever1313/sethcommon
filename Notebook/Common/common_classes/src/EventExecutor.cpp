#include <cstddef> //Needed for NULL
#include <list>
#include <SkyvoThread.h>
#include <iostream>

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
    m_isRunningMutex.unlock();
    m_eventSemaphore.shutdown(); //Allows run thread to exit
    m_exitRunLoopSemaphore.wait(); //Wait for run loop to exit before deleting
    //Run all unexecuted events
    while (!m_eventList.empty()){  //No need to put m_eventList in mutexes, as the run thread has exited.
        executeEvent();
    }
}

void EventExecutor::addEvent(EventInterface *newEvent){
    m_eventListMutex.lock();
    m_eventList.push_back(newEvent);
    m_eventListMutex.unlock();
    m_eventSemaphore.post();
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
        eventToRun->execute(); //Run method
        delete eventToRun;
    }
}

void EventExecutor::run(){
    while (isRunning()){
        m_eventSemaphore.wait();
        executeEvent();
        SkyvoOS::SkyvoThread::sleep(1); ///\todo for some reason, without this here, the program hangs with a few test cases :(
    }
    m_exitRunLoopSemaphore.post();
}

bool EventExecutor::isRunning(){
    bool ret;
    m_isRunningMutex.lock();
    ret = m_isRunning;
    m_isRunningMutex.unlock();
    return ret;
}

}
