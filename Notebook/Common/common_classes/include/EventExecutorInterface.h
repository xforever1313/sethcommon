#ifndef EVENTEXECUTORINTERFACE_H_INCLUDED
#define EVENTEXECUTORINTERFACE_H_INCLUDED

#include "EventInterface.h"

namespace Common{

class EventExecutorInterface{
    public:
        virtual ~EventExecutorInterface(){}
        virtual void addEvent(EventInterface *newEvent) = 0;
};

}

#endif // EVENTEXECUTORINTERFACE_H_INCLUDED
