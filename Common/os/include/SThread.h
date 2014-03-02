
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STHREAD_H
#define STHREAD_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
    //Undefined references are created
#endif

#include "SSemaphore.h"
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

typedef struct SThreadImpl SThreadImpl_t;

class SThread
{
    public:
        enum SThreadStatus{
            NOT_STARTED,
            RUNNING,
            COMPLETED
        };

        SThread();

		#ifndef _MSC_VER
        ///\brief forbid copy constructor (the standard does this)
        SThread(const SThread &other) = delete;
        ///\brief forbid copy constructor (the standard does this)
        SThread& operator=(const SThread &other) = delete;
		#endif

        ///\brief Move Constructor
        SThread(SThread &&other);
        ///\brief this may or may not be needed
        //SThread &operator=(SThread &&other) noexcept;

        ///\brief JOINS and deletes the thread
        ///\note if the destructor has no way to cause the run method to exit, the thread will never exit, and the program will hang
        virtual ~SThread();

        ///\brief starts the thread by calling the run method
        ///\note no-op if thread is already started or completed
        void start();

        ///\note returns false if thread is not started.
        bool joinable() const;

        ///\brief wait for the thread to exit
        ///\note returns if thread not started
        void join();

        ///\note returns if thread is not running
        void detach();

        static unsigned hardware_concurrency();

        static void yield();

        ///\brief causes the thread that CALLS THIS FUNCTION sleep for the given number of millisecs
        static void sleep(unsigned int millisecs);

        ///\brief returns the status of the thread
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

		///\brief what the thread executes.
        void work();

        SSemaphore m_start_semaphore;
        SThreadStatus m_status;

        SMutex m_status_mutex;

        SThreadImpl_t *m_impl; ///<NULL if thread is not started
};

}
#endif 
