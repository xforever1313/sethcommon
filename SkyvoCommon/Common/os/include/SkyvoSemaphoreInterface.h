#ifndef SKYVOSEMAPHOREINTERFACE_H_INCLUDED
#define SKYVOSEMAPHOREINTERFACE_H_INCLUDED

namespace SkyvoOS{

class SkyvoSemaphoreInterface{
    public:
        virtual ~SkyvoSemaphoreInterface(){}
        virtual void post() = 0;
        virtual void wait() = 0;
        virtual bool tryWait() = 0;
        virtual bool timedWait(unsigned long millisecs) = 0;
        virtual void shutdown() = 0;
        virtual bool isShutdown() = 0;
};

}

#endif // SKYVOSEMAPHOREINTERFACE_H_INCLUDED
