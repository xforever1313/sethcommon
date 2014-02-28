
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SKYVOMUTEX_H
#define SKYVOMUTEX_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
#endif

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

