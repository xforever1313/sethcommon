#ifndef SKYVOMUTEX_H
#define SKYVOMUTEX_H

namespace SkyvoOS{

typedef struct impl impl;

class SkyvoMutex
{
    public:
        SkyvoMutex();
        virtual ~SkyvoMutex();
        void lock() const;
        void unlock() const;
    private:
        impl *m_impl;
};

}
#endif // SKYVOMUTEX_H
