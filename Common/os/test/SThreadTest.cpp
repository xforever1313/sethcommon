
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <CppUTest/TestHarness.h>
#include <iostream>

#define private public

#include "SThread.h"
#include "ThreadImpl.h"

TEST_GROUP(SThread){
};

///\brief tests the make sure no-ops happen when the thread is not started
TEST(SThread, noOpTest){
    ThreadImpl *uut = new ThreadImpl;
    CHECK(uut->m_impl == NULL);
    ///For the test to pass, no segfaults should happen
    CHECK(!uut->joinable());
    uut->join();
    uut->detach();
    delete uut;
}

///\brief checks the thread status during various parts of the run method
TEST(SThread, statusTest){
    ThreadImpl *uut = new ThreadImpl;
    CHECK_EQUAL(uut->getStatus(),OS::SThread::NOT_STARTED);
    CHECK_EQUAL(uut->m_numberOfRuns, 0);
    uut->start();
    OS::SThread::sleep(1000);
    CHECK_EQUAL(uut->getStatus(),OS::SThread::RUNNING);
    uut->join();
    CHECK_EQUAL(uut->getStatus(),OS::SThread::COMPLETED);
    CHECK_EQUAL(uut->m_numberOfRuns, 1);
    delete uut;
}

///\brief checks the case where the thread is started twice
TEST(SThread, startedTwiceTest){
    ThreadImpl uut;
    CHECK_EQUAL(uut.getStatus(),OS::SThread::NOT_STARTED);
    uut.start();
    uut.start();
    ///Wait 2 seconds for thread to start running
    OS::SThread::sleep(1000);
    CHECK_EQUAL(uut.getStatus(),OS::SThread::RUNNING);
    uut.join();
    CHECK_EQUAL(uut.getStatus(),OS::SThread::COMPLETED);
    CHECK_EQUAL(uut.m_numberOfRuns, 1);
    uut.start();
    CHECK_EQUAL(uut.m_numberOfRuns, 1);
}

///\brief test the case where a thread is deleted before beings started
TEST(SThread, notStartedTest){
    ThreadImpl *uut = new ThreadImpl;
    delete uut;
    //If program doesn't segfault, success!
}

///\brief tests the see what happens is join is called twice
TEST(SThread, doubleJoinTest){
    ThreadImpl uut;
    CHECK(!uut.joinable());
    uut.start();
    CHECK(uut.joinable());
    uut.join();
    uut.join(); //Program should not crash
    CHECK(!uut.joinable());
}

///\brief mainly here for code coverage.  Ensures the number of threads is greater than zero
TEST(SThread, HardwareConcurrencyTest){
    CHECK(OS::SThread::hardware_concurrency() > 0);
    std::cout << "\nHardware Concurrency: " << OS::SThread::hardware_concurrency() << std::endl;
}

#endif

