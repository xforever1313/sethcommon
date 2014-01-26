#ifndef SKYVOMUTEX_H
#define SKYVOMUTEX_H

#include "SkyvoMutexInterface.h"

namespace SkyvoOS{

typedef struct mutexImpl mutexImpl_t;

class SkyvoMutex : public SkyvoMutexInterface{
    public:
        SkyvoMutex();
        virtual ~SkyvoMutex();
        void lock();
        void unlock();
    private:
        SkyvoMutex(const SkyvoMutex&);
        SkyvoMutex &operator=(const SkyvoMutex&);
        mutexImpl_t *m_impl;
};

}
#endif // SKYVOMUTEX_H