
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Not compatible with emscripten

#include "ConditionVariableWaiter.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace OS {

ConditionVariableWaiter::ConditionVariableWaiter(OS::SConditionVariable *cv) :
    m_cv(cv)
{

}

ConditionVariableWaiter::~ConditionVariableWaiter() {

}

void ConditionVariableWaiter::run() {
    m_cv->wait();
}

}

#endif

