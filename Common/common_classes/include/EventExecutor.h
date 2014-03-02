
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EVENT_EXECUTOR_H
#define EVENT_EXECUTOR_H

#ifdef ASM_JS
    #error "Emscripten does not support threading."
#endif

#include <list>

#include "EventInterface.h"
#include "EventExecutorInterface.h"
#include "SMutex.h"
#include "SSemaphore.h"
#include "SThread.h"

namespace Common{

class EventExecutor : public OS::SThread, public EventExecutorInterface{
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
        OS::SMutex m_eventListMutex;
        OS::SSemaphore m_eventSemaphore;
        OS::SSemaphore m_exitRunLoopSemaphore;

        bool m_isRunning;
        OS::SMutex m_isRunningMutex;
};

}

#endif // EVENT_EXECUTOR_H

