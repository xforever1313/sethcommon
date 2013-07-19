#include <mutex>
#include <thread>

#include "SkyvoMutex.h"

namespace SkyvoOS{

typedef struct mutexImpl{
    mutexImpl() :
        m_mutex(new std::mutex())
    {
    }

    virtual ~mutexImpl(){
        delete m_mutex;
    }

    std::mutex *m_mutex;
}mutexImpl_t;

SkyvoMutex::SkyvoMutex() :
    m_impl(new mutexImpl_t())
{
}

SkyvoMutex::~SkyvoMutex(){
    delete m_impl;
}

void SkyvoMutex::lock() const{
    m_impl->m_mutex->lock();
}

void  SkyvoMutex::unlock() const{
    m_impl->m_mutex->unlock();
}

}
