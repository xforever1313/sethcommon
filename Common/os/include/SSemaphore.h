
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SSEMAPHORE_H
#define SSEMAPHORE_H

#ifdef ASM_JS
    #error "Threading is not supported with Emscripten"
#endif

#include "SSemaphoreInterface.h"

namespace OS{

typedef struct semaphoreImpl semaphoreImpl_t;

/**
* \class SSemaphore
* \brief a wrapper around os semaphore calls.
* \author Seth Hendrick
*/
class SSemaphore : public SSemaphoreInterface{
    public:

        /**
        * \brief default constructor, initial count is at zero
        */
        SSemaphore();

        /**
        * \brief constructor
        * \param startCount - the inital count to set the semaphore at
        */
        SSemaphore(unsigned int initialCount);

        virtual ~SSemaphore();

        /**
        * \brief increments the semaphore count by one
        * If there are processes waiting on the semaphore, one gets unblocked and runs
        */
        void post();

        /**
        * \brief decrements the semaphore count by one
        * If the semaphore count is equal to/greater than zero, the thread is blocked.
        */
        void wait();

        /**
        * \brief decrements the semaphore count if the count is greater than zero
        * If the value is less than, equal to zero, returns false, and thread is not blocked.
        * \return true if count greater than zero or shut down, else return false
        */
        bool tryWait();

        /**
        * \brief decrements the semaphore count if count is greater than zero
        *  If the value is less than, equal to zero, waits for a post, or for the timeout.  If timeout occurs, returns false.
        * \param millisecs timeout to wait for a post
        * \return false if timeout expires, else return true.  Returns true if shutdown.
        */
        bool timedWait(unsigned long millisecs);

        /**
        * \brief unblocks the semaphore, and wait calls no longer block threads
        * \warning once this is called, all waits do not block
        */
        void shutdown();

        /**
        * \brief determines if the semaphore is shutdown or not
        * \return true if semaphore is shutdown, else return false
        */
        bool isShutdown();

        /**
        * \brief returns the semaphore count
        * \return the semaphore count
        */
        unsigned int getSemaphoreCount();

    private:
        //Not copyable (Boost does this)
        SSemaphore(const SSemaphore&);
        SSemaphore &operator=(const SSemaphore&);

        semaphoreImpl_t *m_impl;
        bool m_isShutDown;
};

}
#endif
