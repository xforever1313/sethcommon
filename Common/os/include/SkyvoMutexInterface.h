
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
