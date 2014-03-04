
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCKSSEMAPHORE_H_INCLUDED
#define MOCKSSEMAPHORE_H_INCLUDED

#include <gmock/gmock.h>

#include "SSemaphoreInterface.h"

namespace OS{

class MockSSemaphore : public SSemaphoreInterface{
    public:
        virtual ~MockSSemaphore(){}

        MOCK_METHOD0(post, void());
        MOCK_METHOD0(wait, void());
        MOCK_METHOD0(tryWait, bool());
        MOCK_METHOD1(timedWait, bool(unsigned long millisecs));
        MOCK_METHOD0(shutdown, void());
        MOCK_METHOD0(isShutdown, bool());
};

}

#endif 
