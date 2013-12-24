#include <CppUTest/TestHarness.h>
#include <iostream>

#define private public

#include "SkyvoThread.h"
#include "ThreadImpl.h"

TEST_GROUP(SkyvoThread){
};

///\brief tests the make sure no-ops happen when the thread is not started
TEST(SkyvoThread, noOpTest){
    ThreadImpl *uut = new ThreadImpl;
    CHECK(uut->m_impl == NULL);
    ///For the test to pass, no segfaults should happen
    CHECK(!uut->joinable());
    uut->join();
    uut->detach();
    delete uut;
}

///\brief checks the thread status during various parts of the run method
TEST(SkyvoThread, statusTest){
    ThreadImpl *uut = new ThreadImpl;
    CHECK_EQUAL(uut->getStatus(),SkyvoOS::SkyvoThread::NOT_STARTED);
    CHECK_EQUAL(uut->m_numberOfRuns, 0);
    uut->start();
    SkyvoOS::SkyvoThread::sleep(1000);
    CHECK_EQUAL(uut->getStatus(),SkyvoOS::SkyvoThread::RUNNING);
    uut->join();
    CHECK_EQUAL(uut->getStatus(),SkyvoOS::SkyvoThread::COMPLETED);
    CHECK_EQUAL(uut->m_numberOfRuns, 1);
    delete uut;
}

///\brief checks the case where the thread is started twice
TEST(SkyvoThread, startedTwiceTest){
    ThreadImpl uut;
    CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::NOT_STARTED);
    uut.start();
    uut.start();
    ///Wait 2 seconds for thread to start running
    SkyvoOS::SkyvoThread::sleep(1000);
    CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::RUNNING);
    uut.join();
    CHECK_EQUAL(uut.getStatus(),SkyvoOS::SkyvoThread::COMPLETED);
    CHECK_EQUAL(uut.m_numberOfRuns, 1);
    uut.start();
    CHECK_EQUAL(uut.m_numberOfRuns, 1);
}

///\brief test the case where a thread is deleted before beings started
TEST(SkyvoThread, notStartedTest){
    ThreadImpl *uut = new ThreadImpl;
    delete uut;
    //If program doesn't segfault, success!
}

///\brief tests the see what happens is join is called twice
TEST(SkyvoThread, doubleJoinTest){
    ThreadImpl uut;
    CHECK(!uut.joinable());
    uut.start();
    CHECK(uut.joinable());
    uut.join();
    uut.join(); //Program should not crash
    CHECK(!uut.joinable());
}

///\brief mainly here for code coverage.  Ensures the number of threads is greater than zero
TEST(SkyvoThread, HardwareConcurrencyTest){
    CHECK(SkyvoOS::SkyvoThread::hardware_concurrency() > 0);
    std::cout << "\nHardware Concurrency: " << SkyvoOS::SkyvoThread::hardware_concurrency() << std::endl;
}
