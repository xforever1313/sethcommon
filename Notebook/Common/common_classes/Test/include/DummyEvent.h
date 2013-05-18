#ifndef DUMMYEVENT_H
#define DUMMYEVENT_H

#include "EventInterface.h"
#include "SkyvoMutex.h"

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
        static SkyvoOS::SkyvoMutex ranCountMutex;
};

#endif // DUMMYEVENT_H
