
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Not compatible with emscripten

#include <mutex>

#include "ConditionVariableNotifier.h"
#include "SConditionVariable.h"
#include "SThread.h"

namespace OS {

ConditionVariableNotifier::ConditionVariableNotifier(SConditionVariable *cv,
                                                     bool notifyAll) :
    OS::Runnable<ConditionVariableNotifier>(this),
    m_cv(cv),
    m_notified(false),
    m_notifyAll(notifyAll)
{
}

ConditionVariableNotifier::~ConditionVariableNotifier() {
}

void ConditionVariableNotifier::run() {
    OS::SThread::sleep(500);
    std::lock_guard<std::mutex> lock(m_notifiedMutex);
    if (m_notifyAll) {
        m_cv->notifyAll();
    }
    else {
        m_cv->notifyOne();
    }
    m_notified = true;
}

bool ConditionVariableNotifier::getNotified() {
    std::lock_guard<std::mutex> lock(m_notifiedMutex);
    return m_notified;
}

}

#endif

