
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
