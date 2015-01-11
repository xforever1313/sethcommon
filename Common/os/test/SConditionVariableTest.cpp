
//          Copyright Seth Hendrick 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Do not test if emscripten, it will not link

#include "UnitTest.h"

#include "ConditionVariableNotifier.h"
#include "ConditionVariableWaiter.h"
#include "SConditionVariable.h"

TEST_GROUP(SConditionVariable) {
    TEST_SETUP() {
        m_uut = new OS::SConditionVariable();
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    OS::SConditionVariable *m_uut;
};

/**
 * \brief tests the wait method with a notify one
 */
TEST(SConditionVariable, waitTestOne) {
    OS::ConditionVariableNotifier notifier(m_uut, false);
    notifier.start();
    m_uut->wait();
    CHECK(notifier.getNotified());
    notifier.join();
}

/**
 * \brief tests the wait method with a notify all
 */
TEST(SConditionVariable, waitTestAll) {
    OS::ConditionVariableNotifier notifier(m_uut, true);
    notifier.start();
    m_uut->wait();
    CHECK(notifier.getNotified());
    notifier.join();
}

/**
 * \brief tests the notify one method
 */
TEST(SConditionVariable, notifyOneTest) {
    OS::ConditionVariableWaiter waiter(m_uut);
    waiter.start();

    OS::SThread::sleep(200); // Wait a bit for the thread to get going.

    m_uut->notifyOne();
    waiter.join();
    //Should not hang.
}

/**
 * \brief tests the notify all method
 */
TEST(SConditionVariable, notifyAllTest) {
    OS::ConditionVariableWaiter waiter1(m_uut);
    OS::ConditionVariableWaiter waiter2(m_uut);
    waiter1.start();
    waiter2.start();


    OS::SThread::sleep(200); // Wait a bit for the threads to get going.

    m_uut->notifyAll();
    waiter1.join();
    waiter2.join();
    //Should not hang.
}

TEST(SConditionVariable, timedWaitFalseTest) {
    OS::ConditionVariableNotifier notifier(m_uut, false);
    notifier.start();

    CHECK(!m_uut->timedWait(200)); // Timeout before thing gets notified
    CHECK(!notifier.getNotified());

    notifier.join();
}

TEST(SConditionVariable, timedWaitTrueTest) {
    OS::ConditionVariableNotifier notifier(m_uut, false);
    notifier.start();

    CHECK(m_uut->timedWait(600)); // Timeout after thing gets notified
    CHECK(notifier.getNotified());

    notifier.join();
}

/**
 * \brief tests the shutdown method
 */
TEST(SConditionVariable, shutdownTest) {
    OS::ConditionVariableWaiter waiter1(m_uut);
    OS::ConditionVariableWaiter waiter2(m_uut);
    waiter1.start();
    waiter2.start();

    m_uut->shutdown();
    CHECK(m_uut->isShutdown());
    waiter1.join();
    waiter2.join();
    //Should not hang.
}

/**
 * \brief calls shutdown and then wait.
 *        program should not hang
 */
TEST(SConditionVariable, shutdownWaitTest) {
    m_uut->shutdown();
    CHECK(m_uut->isShutdown());

    m_uut->wait(); // Should not hang
    CHECK(m_uut->timedWait(50000)); // Should not hang.
}

#endif

