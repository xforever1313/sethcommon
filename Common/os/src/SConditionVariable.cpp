
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Will not compile for emscripten

#include <condition_variable>
#include <mutex>

#include "SConditionVariable.h"

namespace OS {

// C++11 impl
struct SConditionVariable::ConditionVariableImpl {
    ConditionVariableImpl() {

    }

    ~ConditionVariableImpl() {

    }

    std::condition_variable m_conditionVariable;
    std::mutex m_lock;

    ConditionVariableImpl(const ConditionVariableImpl&) = delete;
};

SConditionVariable::SConditionVariable() :
    m_impl(new ConditionVariableImpl),
    m_isShutdown(false)
{

}

SConditionVariable::~SConditionVariable() {
    shutdown();
    delete m_impl;
}

void SConditionVariable::notifyOne() {
    m_impl->m_conditionVariable.notify_one();
}

void SConditionVariable::notifyAll() {
    m_impl->m_conditionVariable.notify_all();
}


void SConditionVariable::wait() {
    if (!isShutdown()) {
        std::unique_lock<std::mutex> lock(m_impl->m_lock);
        m_impl->m_conditionVariable.wait(lock);
    }
}

bool SConditionVariable::timedWait(unsigned long millisecs) {
    bool ret = true;
    if (!isShutdown()) {
        std::unique_lock<std::mutex> lock(m_impl->m_lock);
        std::cv_status timeout = m_impl->m_conditionVariable.wait_for(lock,
                                                                      std::chrono::milliseconds(millisecs));

        if (timeout == std::cv_status::timeout) {
            ret = false;
        }
    }

    return ret;
}

void SConditionVariable::shutdown() {
    std::lock_guard<std::mutex> lock(m_isShutdownMutex);
    m_isShutdown = true;
    notifyAll(); //Wake up ALL the threads
}

bool SConditionVariable::isShutdown() {
    std::lock_guard<std::mutex> lock(m_isShutdownMutex);
    return m_isShutdown;
}

}

#endif

