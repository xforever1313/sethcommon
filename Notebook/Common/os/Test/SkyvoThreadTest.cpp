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
}

///\brief checks the thread status during various parts of the run method
BOOST_AUTO_TEST_CASE(SkyvoThread_statusTest){
    ThreadImpl uut;
    BOOST_CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::NOT_STARTED);
    BOOST_CHECK_EQUAL(uut.m_numberOfRuns, 0);
    uut.start();
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

///\brief test the case where a thread is deleted before beings started
BOOST_AUTO_TEST_CASE(SkyvoThread_notStartedTest){
    ThreadImpl *uut = new ThreadImpl;
    delete uut;
    //If program doesn't segfault, success!
}

///\brief tests the see what happens is join is called twice
BOOST_AUTO_TEST_CASE(SkyvoThread_doubleJoinTest){
    ThreadImpl uut;
    BOOST_CHECK(!uut.joinable());
    uut.start();
    BOOST_CHECK(uut.joinable());
    uut.join();
    uut.join(); //Program should not crash
    BOOST_CHECK(!uut.joinable());
}

///\brief mainly here for code coverage.  Ensures the number of threads is greater than zero
BOOST_AUTO_TEST_CASE(SkyvoThread_HardwareConcurrencyTest){
    BOOST_CHECK(SkyvoOS::SkyvoThread::hardware_concurrency() > 0);
    std::cout << "Hardware Concurrency: " << SkyvoOS::SkyvoThread::hardware_concurrency() << std::endl;
}
