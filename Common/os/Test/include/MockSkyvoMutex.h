
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCKSKYVOMUTEX_H_INCLUDED
#define MOCKSKYVOMUTEX_H_INCLUDED

#include <gmock/gmock.h>

#include "SkyvoMutexInterface.h"

namespace SkyvoOS{

class MockSkyvoMutex : public SkyvoMutexInterface{
    public:
        virtual ~MockSkyvoMutex(){}
        MOCK_METHOD0(lock, void());
        MOCK_METHOD0(unlock, void());
};

}

#endif // MOCKSKYVOMUTEX_H_INCLUDED
