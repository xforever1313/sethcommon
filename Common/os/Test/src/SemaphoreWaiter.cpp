
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Not compatible with emscripten

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

#endif

