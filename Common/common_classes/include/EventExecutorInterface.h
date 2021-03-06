
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef EVENTEXECUTORINTERFACE_H_INCLUDED
#define EVENTEXECUTORINTERFACE_H_INCLUDED

#include <memory>

#include "EventInterface.h"

namespace Common{

class EventExecutorInterface{
    public:
        virtual ~EventExecutorInterface(){}
        virtual void startExecutor() = 0;
        virtual void addEvent(const std::shared_ptr<EventInterface> &newEvent) = 0;
};

}

#endif
