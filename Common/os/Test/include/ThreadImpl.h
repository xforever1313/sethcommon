
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef THREADIMPL_H
#define THREADIMPL_H

#include "SkyvoThread.h"

/**
* \class ThreadImpl
* \brief This class is used in SkyvoThreadTest as a class that implements pure virtual methods to test the SkyvoThread class
*/
class ThreadImpl : public SkyvoOS::SkyvoThread{
    public:
        ThreadImpl();
        virtual ~ThreadImpl();
        virtual void run();
        unsigned int m_numberOfRuns;
};

#endif // THREADIMPL_H
