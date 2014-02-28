
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
