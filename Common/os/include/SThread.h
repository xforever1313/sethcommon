
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

#include "SConditionVariable.h"
#include "SMutex.h"

/**
* \class SThread
* \brief This class serves as a wrapper to the os threads
* \author Seth Hendrick
*
* \warning all functions are no-op if the thread is not started (start is not called)
* \warning This file will not compile with the emscripten compiler.
*/
namespace OS{

class SThread
{
    public:
        enum SThreadStatus{
            NOT_STARTED,
            RUNNING,
            COMPLETED
        };

        /**
         * \brief the purpose of the impl is to hide any 
         *        library or standard specific code
         *        in the implementation.
         */
		struct SThreadImpl;
		
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
        virtual ~SThread();

        /**
         * \brief starts the thread by calling the run method
         * \note no-op if thread is already started or completed
         */
        void start();

        /**
         * \note returns false if thread is not started.
         */
        bool joinable() const;

        /**
         * \brief wait for the thread to exit
         * \note returns if thread not started
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

    protected:
        virtual void run() = 0;

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

        SMutex m_status_mutex;

        SThreadImpl *m_impl; ///<nullptr if thread is not started
};

}

#endif

