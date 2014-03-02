
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SMUTEX_H
#define SMUTEX_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
#endif

#include "SMutexInterface.h"

namespace OS{

typedef struct mutexImpl mutexImpl_t;

class SMutex : public SMutexInterface{
    public:
        SMutex();
        virtual ~SMutex();
        void lock();
        void unlock();
    private:
        SMutex(const SMutex&);
        SMutex &operator=(const SMutex&);
        mutexImpl_t *m_impl;
};

}
#endif 

