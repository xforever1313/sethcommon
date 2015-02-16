#include <thread>

#include "UnitTest.h"

#include "ResetEvent.h"
#include "SThread.h"

TEST_GROUP(ResetEventTest) {
    TEST_SETUP() {
        m_uut = new OS::ResetEvent(false);

        CHECK(!m_uut->isSet());
        CHECK(!m_uut->m_isSet);
        CHECK(!m_uut->isShutdown());
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    OS::ResetEvent *m_uut;
};

TEST(ResetEventTest, setTest) {
    m_uut->set();

    CHECK(m_uut->isSet());
    CHECK(m_uut->m_isSet);

    // Ensure wait doesn't hang.
    m_uut->wait();
    CHECK(m_uut->timedWait(10));
}

TEST(ResetEventTest, setWhenWaitingTest) {
    auto func = [&]() {
        m_uut->wait();
    };

    std::thread waitThread(func);
    OS::SThread::sleep(250);
    m_uut->set();

    // Test should not hang.
    waitThread.join();

    CHECK(m_uut->isSet());
    CHECK(m_uut->m_isSet);

}

TEST(ResetEventTest, setWhenWaitingTest2) {
    auto func = [&]() {
        OS::SThread::sleep(250);
        m_uut->wait();
    };

    std::thread waitThread(func);
    m_uut->set();

    // Test should not hang.
    waitThread.join();

    CHECK(m_uut->isSet());
    CHECK(m_uut->m_isSet);

}


TEST(ResetEventTest, timedWaitFalse) {
    bool success = false;
    auto func = [&]() {
        success = !m_uut->timedWait(10);
    };

    std::thread waitThread(func);
    waitThread.join();

    CHECK(success);
}

TEST(ResetEventTest, timedWaitTrue) {
    bool success = false;
    auto func = [&]() {
        success = m_uut->timedWait(10000);
    };

    std::thread waitThread(func);
    m_uut->set();
    waitThread.join();

    CHECK(success);
}


TEST(ResetEventTest, setResetTimedWaitTest) {
    // First, start the thread and set it.
    auto func = [&]() {
        m_uut->wait();
    };

    std::thread waitThread(func);

    m_uut->set();

    // Test should not hang.
    waitThread.join();

    CHECK(m_uut->isSet());
    CHECK(m_uut->m_isSet);


    // Run it again.  Should blow through it.
    std::thread waitThread2(func);
    waitThread2.join();

    // Reset
    m_uut->reset();
    CHECK(!m_uut->isSet());
    CHECK(!m_uut->m_isSet);


    // Should wait.  When set, it should return true.
    bool success = false;
    auto func2 = [&]() {
        success = m_uut->timedWait(1000);
    };

    std::thread waitThread3(func2);
    m_uut->set();
    waitThread3.join();

    CHECK(success);
    CHECK(m_uut->isSet());
    CHECK(m_uut->m_isSet);
}

TEST(ResetEventTest, shutdownTest) {
    m_uut->reset();

    // First, start the thread and set it.
    auto func = [&]() {
        m_uut->wait();
    };

    std::thread waitThread(func);

    OS::SThread::sleep(250);

    m_uut->shutdown();
    waitThread.join();

    // should not wait
    m_uut->wait();
    CHECK(m_uut->timedWait(10));
}
