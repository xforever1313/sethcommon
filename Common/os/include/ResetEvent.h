//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RESET_EVENT_H_
#define RESET_EVENT_H_

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
#endif

#include <condition_variable>
#include <mutex>

namespace OS {

/**
 * \brief This class acts like C#'s ManualResetEvent.
 *        When wait is called, the calling thread will wait
 *        until set it called.  Further callings to wait will not
 *        wait until reset() is called.
 */
class ResetEvent {
    public:
        /**
         * \brief Constructor
         * \param whether or not the reset is already set or not.
         */
        explicit ResetEvent(bool isSet = false);

        /**
         * \brief Shuts down this reset event. All threads
         *        waiting become unblocked.
         */
        ~ResetEvent();

        /**
         * \brief Sets the reset event.  All threads waiting
         *        are notified to continue running.
         *        All future calls to wait will not wait until
         *        reset is called.
         */
        void set();

        /**
         * \brief Resets the reset event.  All future
         *        calls to wait will cause the thread
         *        to wait until set is called.
         */
        void reset();

        /**
         * \brief The calling thread will wait
         *        until set is called.  However,
         *        if the reset event is already
         *        set, the thread will not wait.
         */
        void wait();

        /**
         * \brief will wait either until the
         *        reset event is set, or the
         *        passed in time expires. Will not
         *        wait if the event is already set.
         * \param millisecs How many milliseconds
         *        to wait for.
         * \return true if the event is set while waiting,
         *         or the event is already set.  False if it timesout.
         */
        bool timedWait(unsigned long millisecs);

        /**
         * \return true is the reset is set, false otherwise.
         */
        bool isSet();

        /**
         * \return true is this is shutdown, false otherwise.
         */
        bool isShutdown();

        /**
         * \brief Shuts down the reset event by waking up
         *        all threads.  All future calls to wait
         *        not wait, regardless if the event is set or not.
         */
        void shutdown();

    private:
        bool m_isSet;
        bool m_isShutdown;
        std::mutex m_isSetMutex;
        std::mutex m_isShutdownMutex;
        std::condition_variable m_cv;
};

}

#endif

