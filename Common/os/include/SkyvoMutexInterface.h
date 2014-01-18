#ifndef SKYVOMUTEXINTERFACE_H_INCLUDED
#define SKYVOMUTEXINTERFACE_H_INCLUDED

namespace SkyvoOS{

class SkyvoMutexInterface{
    public:
        virtual ~SkyvoMutexInterface(){}
        virtual void lock() = 0;
        virtual void unlock() = 0;
};

}

#endif // SKYVOMUTEXINTERFACE_H_INCLUDED
