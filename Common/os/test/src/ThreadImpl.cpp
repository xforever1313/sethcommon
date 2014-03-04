
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Not compatible with emscripten

#include "ThreadImpl.h"

ThreadImpl::ThreadImpl() :
    m_numberOfRuns(0)
{
}

ThreadImpl::~ThreadImpl()
{
}

void ThreadImpl::run(){
    SThread::sleep(1500);
    ++m_numberOfRuns;
}

#endif
