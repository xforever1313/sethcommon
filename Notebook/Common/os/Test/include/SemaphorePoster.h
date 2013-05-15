#ifndef SEMAPHOREPOSTER_H
#define SEMAPHOREPOSTER_H

/**
* \class SemaphorePoster
* \brief The only purpose of this class is to post the passed in semaphore when start is called after half a second for testing purposes
* \author Seth Hendrick
*/

#include "SkyvoSemaphore.h"
#include "SkyvoThread.h"

class SemaphorePoster : public SkyvoOS::SkyvoThread
{
    public:
        SemaphorePoster(SkyvoOS::SkyvoSemaphore *semaphore);
        virtual ~SemaphorePoster();
        void run();
        bool getPosted();
    private:
        SemaphorePoster();

        SkyvoOS::SkyvoSemaphore *m_semaphore;
        bool m_posted; ///<Whether or not the thing was posted to
        SkyvoOS::SkyvoMutex m_posted_mutex;
};

#endif // SEMAPHOREPOSTER_H
