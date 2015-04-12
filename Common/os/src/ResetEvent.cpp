//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "ResetEvent.h"

#ifndef ASM_JS //Will not compile for emscripten

namespace OS {

ResetEvent::ResetEvent(bool isSet /*= false*/) :
    m_isSet(isSet),
    m_isShutdown(false)
{
}

ResetEvent::~ResetEvent() {
    shutdown();
}

void ResetEvent::set() {
    std::lock_guard<std::mutex> lock(m_isSetMutex);
    m_isSet = true;
    m_cv.notify_all();
}

void ResetEvent::reset() {
    std::lock_guard<std::mutex> lock(m_isSetMutex);
    m_isSet = false;
}

void ResetEvent::wait() {
    if (!isShutdown()) {
        std::unique_lock<std::mutex> lock(m_isSetMutex);
        if (!m_isSet) {
            m_cv.wait(lock);
        }
    }
}

bool ResetEvent::timedWait(unsigned long millisecs) {
    bool ret = true;

    if (!isShutdown()) {
        std::unique_lock<std::mutex> lock(m_isSetMutex);
        if (!m_isSet) {
            std::cv_status timeout = m_cv.wait_for(lock,
                                                   std::chrono::milliseconds(millisecs));

            if (timeout == std::cv_status::timeout) {
                ret = false;
            }
        }
    }
    return ret;
}

bool ResetEvent::isSet() {
    std::lock_guard<std::mutex> lock(m_isSetMutex);
    return m_isSet;
}

bool ResetEvent::isShutdown() {
    std::lock_guard<std::mutex> lock(m_isShutdownMutex);
    return m_isShutdown;
}

void ResetEvent::shutdown() {
    std::lock_guard<std::mutex> lock(m_isShutdownMutex);
    m_isShutdown = true;
    set();
}

}

#endif
