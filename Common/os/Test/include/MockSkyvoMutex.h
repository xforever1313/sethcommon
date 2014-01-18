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
