
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EVENT_EXECUTOR_H
#define EVENT_EXECUTOR_H

#ifdef ASM_JS
    #error "Emscripten does not support threading."
#endif

#include <memory>
#include <queue>

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
         * \param newEvent A shared pointer to the Event that needs to be added.
         *                 It was decided a shared pointer should be used instead
         *                 of a raw pointer since EventExecutor should not
         *                 own an event, it just has a reference to them.
         *                 Before, EventExecutor would delete an event once
         *                 it was done executing it, but this was a bad idea
         *                 since what if someone wanted to keep the event object
         *                 after it executed?  Shared Pointers allow this.
         */
        void addEvent(const std::shared_ptr<EventInterface> &newEvent) override;

        #ifdef UNIT_TEST
        static bool startRightAway; ///<True to start right away (default)
        #endif

    private:
        void executeEvent();
        void run();
        bool isRunning();

        std::queue<std::shared_ptr<EventInterface> > m_eventList;
        OS::SMutex m_eventListMutex;
        OS::SSemaphore m_eventSemaphore;
        OS::SSemaphore m_exitRunLoopSemaphore;

        bool m_isRunning;
        OS::SMutex m_isRunningMutex;
};

}

#endif // EVENT_EXECUTOR_H
