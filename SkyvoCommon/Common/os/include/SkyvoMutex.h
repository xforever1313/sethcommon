#ifndef SKYVOMUTEX_H
#define SKYVOMUTEX_H

namespace SkyvoOS{

typedef struct mutexImpl mutexImpl_t;

class SkyvoMutex
{
    public:
        SkyvoMutex();
        virtual ~SkyvoMutex();
        void lock() const;
        void unlock() const;
    private:
        mutexImpl_t *m_impl;
};

}
#endif // SKYVOMUTEX_H