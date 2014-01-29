#ifndef ASM_JS //Only used in exectutor test, which is not usable with emscripten
#include <SkyvoThread.h>


#include "DummyEvent.h"

unsigned int DummyEvent::ranCount = 0;
SkyvoOS::SkyvoMutex DummyEvent::ranCountMutex;
SkyvoOS::SkyvoSemaphore DummyEvent::semaphore;
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

