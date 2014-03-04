
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCKEVENTEXECUTOR_H_INCLUDED
#define MOCKEVENTEXECUTOR_H_INCLUDED

#include <gmock/gmock.h>

#include "EventInterface.h"
#include "EventExecutorInterface.h"

class MockEventExecutor : public Common::EventExecutorInterface{
    public:
        virtual ~MockEventExecutor(){}
        MOCK_METHOD1(addEvent, void(Common::EventInterface *newEvent));
};

#endif // MOCKEVENTEXECUTOR_H_INCLUDED
