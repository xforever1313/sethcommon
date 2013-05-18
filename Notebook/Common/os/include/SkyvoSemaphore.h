#ifndef SKYVOSEMAPHORE_H
#define SKYVOSEMAPHORE_H

#include "SkyvoMutex.h"

namespace SkyvoOS{

typedef struct impl impl;

/**
* \class SkyvoSemaphore
* \brief a wrapper around os semaphore calls.
* \detail boost documentation: http://www.boost.org/doc/libs/1_53_0/boost/interprocess/sync/interprocess_semaphore.hpp
* \author Seth Hendrick
*/
class SkyvoSemaphore{
    public:

        /**
        * \brief default constructor, initial count is at zero
        */
        SkyvoSemaphore();

        /**
        * \brief constructor
        * \param startCount - the inital count to set the semaphore at
        */
        SkyvoSemaphore(unsigned int initialCount);

        virtual ~SkyvoSemaphore();

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
        *  the value is less than, equal to zero, waits for a post, or for the timeout.  If timeout occurs, returns false.
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
        int getSemaphoreCount();

    private:
        //Not copyable (Boost does this)
        SkyvoSemaphore(const SkyvoSemaphore&);
        SkyvoSemaphore &operator=(const SkyvoSemaphore&);

        impl *m_impl;
        bool m_isShutDown;
        SkyvoMutex m_isShutdownMutex;

};

}
#endif // SKYVOSEMAPHORE_H
