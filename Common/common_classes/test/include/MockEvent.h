#ifndef MOCK_EVENT_H_
#define MOCK_EVENT_H_

#include <gmock/gmock.h>

#include "EventInterface.h"

namespace Common{

class MockEvent : public EventInterface {
    public:
        virtual ~MockEvent(){}
        MOCK_METHOD0(execute, void());
};

}

#endif

