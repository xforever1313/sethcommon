
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SEMAPHOREPOSTER_H
#define SEMAPHOREPOSTER_H

/**
* \class SemaphorePoster
* \brief The only purpose of this class is to post the passed in semaphore when start is called after half a second for testing purposes
* \author Seth Hendrick
*/

#include "SSemaphore.h"
#include "SThread.h"

class SemaphorePoster : public OS::SThread
{
    public:
        SemaphorePoster(OS::SSemaphore *semaphore);
        virtual ~SemaphorePoster();
        void run();
        bool getPosted();
    private:
        SemaphorePoster();

        OS::SSemaphore *m_semaphore;
        bool m_posted; ///<Whether or not the thing was posted to
        OS::SMutex m_posted_mutex;
};

#endif // SEMAPHOREPOSTER_H
