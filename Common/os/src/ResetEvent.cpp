//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mutex>

#include "ResetEvent.h"
#include "SConditionVariable.h"
#include "SMutex.h"

#ifndef ASM_JS //Will not compile for emscripten

namespace OS {

ResetEvent::ResetEvent(bool isSet /*= false*/) :
    m_isSet(isSet)
{
}

ResetEvent::~ResetEvent() {
    shutdown();
}

void ResetEvent::set() {
    std::lock_guard<SMutex> lock(m_isSetMutex);
    m_isSet = true;
    m_cv.notifyAll();
}

void ResetEvent::reset() {
    std::lock_guard<SMutex> lock(m_isSetMutex);
    m_isSet = false;
}

void ResetEvent::wait() {
    if (!isSet() && !isShutdown()) {
        m_cv.wait();
    }
}

bool ResetEvent::timedWait(unsigned long millisecs) {
    bool ret = true;

    if (!isSet() && !isShutdown()) {
        ret = m_cv.timedWait(millisecs);
    }

    return ret;
}

bool ResetEvent::isSet() {
    std::lock_guard<SMutex> lock(m_isSetMutex);
    return m_isSet;
}

bool ResetEvent::isShutdown() {
    return m_cv.isShutdown();
}

void ResetEvent::shutdown() {
    m_cv.shutdown();
}

}

#endif
