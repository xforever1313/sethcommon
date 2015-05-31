
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STHREAD_H
#define STHREAD_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
    //Undefined references are created
#endif

#include <functional>
#include <mutex>
#include <thread>

#include "SConditionVariable.h"

/**
* \class SThread
* \brief This class serves as a wrapper to the os threads
* \author Seth Hendrick
*
* \warning all functions are no-op if the thread is not started (start is not called)
* \warning This file will not compile with the emscripten compiler.
*/
namespace OS{

class SThread {
    public:
        enum SThreadStatus{
            NOT_STARTED,
            RUNNING,
            COMPLETED,
            JOINED
        };

        /**
        * \brief Just creates an instance of this class.
        *       does not start any threads.  Use start() for that.
        */
        SThread();

		#ifndef _MSC_VER
        /**
         *\brief forbid copy constructor (the standard does this)
         */
        SThread(const SThread &other) = delete;

        /**
         * \brief forbid copy constructor (the standard does this)\
         */
        SThread& operator=(const SThread &other) = delete;
		#endif

        /**
         *\brief JOINS and deletes the thread
         *\warning if the destructor has no way to cause the run method to exit,
         *         the thread will never exit, and the program will hang
         */
        ~SThread();

        /**
         * \brief starts the thread by calling the run method
         * \param runFunc The thread to run.
         * \note no-op if thread is already started or completed
         */
        void start(const std::function<void()> &runFunc);

        /**
         * \note returns false if thread is not started.
         */
        bool joinable();

        /**
        * \brief wait for the thread to exit
        * \note returns if thread not started
        * \note Unlike std::thread, you do not need to check joinable()
        *       before calling this.  This method takes care of that for you!
        */
        void join();

        ///\note returns if thread is not running
        void detach();

        static unsigned hardware_concurrency();

        static void yield();

        /**
         * \brief causes the thread that CALLS THIS FUNCTION to
         * sleep for the given number of millisecs
         */
        static void sleep(unsigned int millisecs);

        /**
         * \brief returns the status of the thread
         */
        SThreadStatus getStatus();

    private:
        #ifdef _MSC_VER
        ///\brief forbid copy constructor (the standard does this)
        SThread(const SThread &other);
        ///\brief forbid copy constructor (the standard does this)
        SThread& operator=(const SThread &other);
		#endif

		/**
         * \brief what the thread executes.
         */
        void work();

        SConditionVariable m_startCV;
        SThreadStatus m_status;

        std::mutex m_status_mutex;
        std::thread m_thread;
        std::function<void()> m_runFunc; ///< The function to run.
};

/**
 * \brief Make a class inherit from this to make it act like
 *       Java's Runnable.  Inherit using:
 *       class myClass : public Runnable<myClass>.
 *       You will also need to make sure that myClass has a run function
 *       implemented.  In the Constructor for your class, do:
 *       myClass::myClass() :
 *          OS::Runnable<myClass>(this).
 *       run() will also need to be public.
 */
template <class T>
class Runnable {
    public:
        explicit Runnable(T *base) :
            m_base(base)
        {
        };

        virtual ~Runnable() {
        }

        void start() {
            m_thread.start(std::bind(&T::run, m_base));
        }

        void join() {
            m_thread.join();
        }

    protected:
        Runnable() = delete;

        SThread m_thread;
        T *m_base;
};

}

#endif

