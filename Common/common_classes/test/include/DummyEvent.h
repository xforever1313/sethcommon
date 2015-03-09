
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef DUMMYEVENT_H
#define DUMMYEVENT_H

#include <mutex>

#include "EventInterface.h"
#include "SSemaphore.h"

class DummyEvent : public Common::EventInterface{
    public:
        DummyEvent();
        virtual ~DummyEvent();
        void execute();

        /**
        * \brief safely get ran count via mutexes
        */
        static unsigned int getRanCount();

        static unsigned int ranCount; //Number of times execute was run for all DummyEvents
        static std::mutex ranCountMutex;
        static OS::SSemaphore semaphore;
};

#endif // DUMMYEVENT_H
