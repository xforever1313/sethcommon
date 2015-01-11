
//          Copyright Seth Hendrick 2015.
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

class SMutex : public SMutexInterface{
    public:
        /**
         * \brief the purpose of the impl is to hide any 
         *        library or standard specific code
         *        in the implementation.
         */
	    struct MutexImpl;

        SMutex();

        ~SMutex();

        void lock();
        void unlock();

    private:
        SMutex(const SMutex&);
        SMutex &operator=(const SMutex&);
        MutexImpl *m_impl;
};

}

#endif 

