
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Only used in exectutor test, which is not usable with emscripten
#include "SThread.h"


#include "DummyEvent.h"

unsigned int DummyEvent::ranCount = 0;
OS::SMutex DummyEvent::ranCountMutex;
OS::SSemaphore DummyEvent::semaphore;
DummyEvent::DummyEvent()
{
}

DummyEvent::~DummyEvent(){
}

void DummyEvent::execute(){
    DummyEvent::ranCountMutex.lock();
    ++DummyEvent::ranCount;
    DummyEvent::semaphore.post();
    DummyEvent::ranCountMutex.unlock();
}

unsigned int DummyEvent::getRanCount(){
    unsigned int ret;
    DummyEvent::ranCountMutex.lock();
    ret = DummyEvent::ranCount;
    DummyEvent::ranCountMutex.unlock();
    return ret;
}

#endif

