//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCONDITION_VARIABLE_H
#define SCONDITION_VARIABLE_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
#endif

#include <condition_variable>
#include <mutex>

namespace OS{

/**
* \class SConditionVariable
* \brief a wrapper around os Condition Variable calls.
* \author Seth Hendrick
*/
class SConditionVariable {
    public:

        /**
         * \brief default constructor, initial count is at zero
         */
        SConditionVariable();

        /**
         * \brief calls shutdown().
         */
        ~SConditionVariable();

        /**
         * \brief If there are processes waiting on the condition variable,
         *        one gets unblocked and runs
         */
        void notifyOne();

        /**
         * \brief Wakes up ALL threads waiting on the condition variable.
         *        However, unlike shutdown() this does not prevent
         *        another thread from calling wait and waiting on it.
         */
        void notifyAll();

        /**
         * \brief A thread that calls this will wait until notify(), notifyAll() or
         *        shutdown() is called.  If the condition variable is shutdown,
         *        this method will no longer block.
         */
        void wait();

        /**
        * \brief The calling thread waits until a notify occurs, or a timeout occurs;
        *        whichever happens first.
        * \param millisecs timeout to wait for a post
        * \return false if timeout expires, else return true.  Returns true if shutdown.
        */
        bool timedWait(unsigned long millisecs);

        /**
        * \brief Does a notify_all(), and future calls to wait()
        *        or timed_wait() does not block.
        * \warning once this is called, all waits do not block
        */
        void shutdown();

        /**
        * \brief determines if the condition variable is shutdown or not
        * \return true if the condition variable is shutdown, else return false
        */
        bool isShutdown();

    private:
        //Not copyable
        SConditionVariable(const SConditionVariable&) = delete;
        SConditionVariable &operator=(const SConditionVariable&) = delete;

        bool m_isShutdown;
        std::mutex m_isShutdownMutex;
        std::mutex m_cvLock;
        std::condition_variable m_conditionVariable;
};

}

#endif

