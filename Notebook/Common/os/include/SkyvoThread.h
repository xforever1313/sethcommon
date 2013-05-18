#ifndef SKYVOTHREAD_H
#define SKYVOTHREAD_H

#include "SkyvoMutex.h"

/**
* \class SkyvoThread
* \brief This class serves as a wrapper to the os threads
* \author Seth Hendrick
*
* Boost thread documentation: http://www.boost.org/doc/libs/1_53_0/doc/html/thread/thread_management.html
* \warning all functions are no-op if the thread is not started (start is not called)
*/
namespace SkyvoOS{

typedef struct skyvoThreadImpl skyvoThreadImpl_t;

class SkyvoThread
{
    public:
        enum SkyvoThreadStatus{
            NOT_STARTED,
            RUNNING,
            COMPLETED
        };

        SkyvoThread();

        ///\brief forbid copy constructor (boost does this)
        SkyvoThread(const SkyvoThread &other) = delete;
        ///\brief forbid copy constructor (boost does this)
        SkyvoThread& operator=(const SkyvoThread &other) = delete;

        ///\brief Move Constructor
        SkyvoThread(SkyvoThread &&other) noexcept;
        ///\brief this may or may not be needed
        //SkyvoThread &operator=(SkyvoThread &&other) noexcept;

        ///\brief JOINS and deletes the thread
        ///\note if the destructor has no way to cause the run method to exit, the thread will never exit, and the program will hang
        virtual ~SkyvoThread();

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

        ///\note returns if thread is not running
        void interrupt();

        ///\note returns false if thread not started
        bool interruption_requested() const;

        ///\brief causes the thread that CALLS THIS FUNCTION sleep for the given number of millisecs
        static void sleep(unsigned int millisecs);

        ///\brief returns the status of the thread
        SkyvoThreadStatus getStatus() const;

    protected:
        virtual void run() = 0;

    private:
        ///\brief what the thread executes.
        void work();

        SkyvoThreadStatus m_status;

        SkyvoMutex m_status_mutex;

        skyvoThreadImpl_t *m_impl; ///<NULL if thread is not started
};

}
#endif // SKYVOTHREAD_H
