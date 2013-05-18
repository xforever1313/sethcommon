#include <SkyvoThread.h>

#include "DummyEvent.h"
unsigned int DummyEvent::ranCount = 0;
SkyvoOS::SkyvoMutex DummyEvent::ranCountMutex;
DummyEvent::DummyEvent()
{
    //ctor
}

DummyEvent::~DummyEvent()
{

}

void DummyEvent::execute(){
    DummyEvent::ranCountMutex.lock();
    ++DummyEvent::ranCount;
    DummyEvent::ranCountMutex.unlock();
}

unsigned int DummyEvent::getRanCount(){
    unsigned int ret;
    DummyEvent::ranCountMutex.lock();
    ret = DummyEvent::ranCount;
    DummyEvent::ranCountMutex.unlock();
    return ret;
}
