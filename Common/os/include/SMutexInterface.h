
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SMUTEXINTERFACE_H_INCLUDED
#define SMUTEXINTERFACE_H_INCLUDED

namespace OS{

class SMutexInterface{
    public:
        virtual ~SMutexInterface(){}
        virtual void lock() = 0;
        virtual void unlock() = 0;
};

}

#endif 
