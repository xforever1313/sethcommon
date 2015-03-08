
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <CppUTest/TestHarness.h>
#include <iostream>

#define private public

#include "SThread.h"

TEST_GROUP(SThread){
    TEST_SETUP() {
        m_uut = new OS::SThread();
        m_hasRun = false;
        CHECK_EQUAL(m_uut->getStatus(), OS::SThread::NOT_STARTED);
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    void hasRun() {
        m_hasRun = true;
        OS::SThread::sleep(500);
    }

    bool m_hasRun;
    OS::SThread *m_uut;
};

/**
 * \brief tests the make sure no-ops happen when the thread is not started
 */
TEST(SThread, noOpTest){
    ///For the test to pass, no segfaults should happen
    CHECK(!m_uut->joinable());
    m_uut->join();
    m_uut->detach();
}

/**
 * \brief checks the thread status during various parts of the run method
 */
TEST(SThread, statusTest){
    CHECK_EQUAL(m_uut->getStatus(),OS::SThread::NOT_STARTED);
    CHECK(!m_hasRun);

    m_uut->start(std::bind(&CppUTestGroupSThread::hasRun, this));
    CHECK_EQUAL(m_uut->getStatus(),OS::SThread::RUNNING);
    m_uut->join();

    CHECK_EQUAL(m_uut->getStatus(),OS::SThread::COMPLETED);
    CHECK(m_hasRun);
}

/**
 * \brief checks the case where the thread is started twice
 */
TEST(SThread, startedTwiceTest){
    CHECK_EQUAL(m_uut->getStatus(),OS::SThread::NOT_STARTED);

    m_uut->start(std::bind(&CppUTestGroupSThread::hasRun, this));
    m_uut->start(std::bind(&CppUTestGroupSThread::hasRun, this));

    CHECK_EQUAL(m_uut->getStatus(),OS::SThread::RUNNING);
    m_uut->join();

    CHECK_EQUAL(m_uut->getStatus(),OS::SThread::COMPLETED);
    CHECK(m_hasRun);

    // Ensure start doesn't restart the thread.
    m_hasRun = false;
    m_uut->start(std::bind(&CppUTestGroupSThread::hasRun, this));
    CHECK(!m_hasRun);
}

/**
 * \brief test the case where a thread is deleted before beings started
 */
TEST(SThread, notStartedTest){
    //If program doesn't segfault, success!
}

/**
 *\brief tests the see what happens is join is called twice
 */
TEST(SThread, doubleJoinTest){
    CHECK(!m_uut->joinable());
    m_uut->start(std::bind(&CppUTestGroupSThread::hasRun, this));
    CHECK(m_uut->joinable());
    m_uut->join();
    m_uut->join(); //Program should not crash
    CHECK(!m_uut->joinable());
}

/**
 * \brief mainly here for code coverage.  Ensures the number of threads is greater than zero
 */
TEST(SThread, HardwareConcurrencyTest){
    CHECK(OS::SThread::hardware_concurrency() > 0);
    std::cout << "\nHardware Concurrency: " << OS::SThread::hardware_concurrency() << std::endl;
}

#endif

