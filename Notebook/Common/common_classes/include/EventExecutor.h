#ifndef EVENT_EXECUTOR_H
#define EVENT_EXECUTOR_H

#include <SkyvoMutex.h>
#include <SkyvoSemaphore.h>
#include <SkyvoThread.h>
#include <list>

#include "EventInterface.h"

namespace Common{

class EventExecutor : public SkyvoOS::SkyvoThread{
    public:
        EventExecutor();
        virtual ~EventExecutor();

        /**
        * \warning newEvent is deleted after being executed.
        * It is recommended that you set whatever you pass in to NULL after calling this method, or letting the pointer go out of scope
        */
        void addEvent(EventInterface *newEvent);

        #ifdef UNIT_TEST
        static bool startRightAway; ///<True to start right away (default)
        #endif

    private:
        void executeEvent();
        void run();
        bool isRunning();

        std::list<EventInterface*> m_eventList;
        SkyvoOS::SkyvoMutex m_eventListMutex;
        SkyvoOS::SkyvoSemaphore m_eventSemaphore;
        SkyvoOS::SkyvoSemaphore m_exitRunLoopSemaphore;

        bool m_isRunning;
        SkyvoOS::SkyvoMutex m_isRunningMutex;
};

}

#endif // EVENT_EXECUTOR_H