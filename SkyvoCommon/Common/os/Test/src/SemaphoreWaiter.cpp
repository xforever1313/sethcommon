#include "SemaphoreWaiter.h"
#include "SkyvoSemaphore.h"

SemaphoreWaiter::SemaphoreWaiter(SkyvoOS::SkyvoSemaphore *semaphore) :
    m_semaphore(semaphore)
{
}

SemaphoreWaiter::~SemaphoreWaiter(){
}

void SemaphoreWaiter::run(){
    m_semaphore->wait();
}