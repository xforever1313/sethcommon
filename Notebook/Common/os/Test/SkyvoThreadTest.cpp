#include <boost/test/unit_test.hpp>

#define private public

#include "SkyvoThread.h"
#include "ThreadImpl.h"

///\brief tests the make sure no-ops happen when the thread is not started
BOOST_AUTO_TEST_CASE(SkyvoThread_noOpTest){
    ThreadImpl uut;
    BOOST_CHECK(uut.m_impl == NULL);
    ///For the test to pass, no segfaults should happen
    BOOST_CHECK(!uut.joinable());
    uut.join();
    uut.detach();
    uut.interrupt();
    BOOST_CHECK(!uut.interruption_requested());

}

///\brief checks the thread status during various parts of the run method
BOOST_AUTO_TEST_CASE(SkyvoThread_statusTest){
    ThreadImpl uut;
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::NOT_STARTED);
    BOOST_CHECK_EQUAL(uut.m_numberOfRuns, 0);
    uut.start();
    ///Wait 2 seconds for thread to start running
    SkyvoOS::SkyvoThread::sleep(1000);
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::RUNNING);
    uut.join();
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::COMPLETED);
    BOOST_CHECK_EQUAL(uut.m_numberOfRuns, 1);
}

///\brief checks the case where the thread is started twice
BOOST_AUTO_TEST_CASE(SkyvoThread_startedTwiceTest){
    ThreadImpl uut;
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::NOT_STARTED);
    uut.start();
    uut.start();
    ///Wait 2 seconds for thread to start running
    SkyvoOS::SkyvoThread::sleep(1000);
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::RUNNING);
    uut.join();
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::COMPLETED);
    BOOST_CHECK_EQUAL(uut.m_numberOfRuns, 1);
    uut.start();
    BOOST_CHECK_EQUAL(uut.m_numberOfRuns, 1);
}
