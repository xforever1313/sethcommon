#include <boost/thread.hpp>

#include "SkyvoMutex.h"

namespace SkyvoOS{

typedef struct mutexImpl{
    mutexImpl() :
        m_mutex(new boost::mutex())
    {
    }

    ~mutexImpl(){
        delete m_mutex;
    }

    boost::mutex *m_mutex;
}mutexImpl_t;

SkyvoMutex::SkyvoMutex() :
    m_impl(new mutexImpl_t())
{
}

SkyvoMutex::~SkyvoMutex(){
    unlock();
    delete m_impl;
}

void SkyvoMutex::lock() const{
    m_impl->m_mutex->lock();
}

void  SkyvoMutex::unlock() const{
    m_impl->m_mutex->unlock();
}

}
