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
