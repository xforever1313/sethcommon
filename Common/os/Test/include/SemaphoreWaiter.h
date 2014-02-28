
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SEMAPHOREWAITER_H
#define SEMAPHOREWAITER_H

#include "SkyvoSemaphore.h"
#include "SkyvoThread.h"

/**
* \class SemaphoreWaiter
* \brief The only point of this class is to call Semaphore->wait() in the run method.  Used for testing purposes
*/
class SemaphoreWaiter : public SkyvoOS::SkyvoThread{
    public:
        SemaphoreWaiter(SkyvoOS::SkyvoSemaphore *semaphore);
        virtual ~SemaphoreWaiter();
        void run();

    private:
        SemaphoreWaiter();
        SkyvoOS::SkyvoSemaphore *m_semaphore;
};

#endif // SEMAPHOREWAITER_H
