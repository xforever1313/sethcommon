#ifndef ASM_JS //Not compatible with emscripten

#include "SemaphorePoster.h"

SemaphorePoster::SemaphorePoster(SkyvoOS::SkyvoSemaphore *semaphore) :
    m_semaphore(semaphore),
    m_posted(false)
{
}

SemaphorePoster::~SemaphorePoster(){
}

void SemaphorePoster::run(){
    SkyvoOS::SkyvoThread::sleep(500);
    m_posted_mutex.lock();
    m_semaphore->post();
    m_posted = true;
    m_posted_mutex.unlock();
}

bool SemaphorePoster::getPosted(){
    m_posted_mutex.lock();
    bool posted = m_posted;
    m_posted_mutex.unlock();
    return posted;
}

#endif

