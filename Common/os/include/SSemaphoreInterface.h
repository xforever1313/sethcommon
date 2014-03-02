
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SSEMAPHOREINTERFACE_H_INCLUDED
#define SSEMAPHOREINTERFACE_H_INCLUDED

namespace OS{

class SSemaphoreInterface{
    public:
        virtual ~SSemaphoreInterface(){}
        virtual void post() = 0;
        virtual void wait() = 0;
        virtual bool tryWait() = 0;
        virtual bool timedWait(unsigned long millisecs) = 0;
        virtual void shutdown() = 0;
        virtual bool isShutdown() = 0;
};

}

#endif
