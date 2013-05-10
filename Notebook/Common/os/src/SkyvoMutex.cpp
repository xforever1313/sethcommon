#include <boost/thread/mutex.hpp>

#include "SkyvoMutex.h"

namespace SkyvoOS{

typedef struct impl{
    impl() :
        m_mutex(new boost::mutex())
    {
    }

    ~impl(){
        delete m_mutex;
    }

    boost::mutex *m_mutex;
}impl;

SkyvoMutex::SkyvoMutex() :
    m_impl(new impl())
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
