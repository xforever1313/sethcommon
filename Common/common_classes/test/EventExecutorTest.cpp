
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <CppUTest/TestHarness.h>
#include <memory>

#define private public

#include "DummyEvent.h"
#include "EventExecutor.h"
#include "EventInterface.h"

TEST_GROUP(EventExecutor){
    TEST_SETUP(){
        DummyEvent::ranCount = 0;
    }
};

/**
 * \brief tests the run method
 */
TEST(EventExecutor, runTest){
    //Have a mix of shared pointers in different scopes.
    std::shared_ptr<Common::EventInterface> event1(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event2(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event3(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event4(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event5(new DummyEvent());

    Common::EventExecutor uut;
    CHECK(!uut.isRunning());

    uut.startExecutor();
    CHECK(uut.isRunning());

    uut.addEvent(event1);
    uut.addEvent(event2);
    uut.addEvent(event3);
    uut.addEvent(event4);
    uut.addEvent(event5);
    uut.addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut.addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut.addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut.addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    uut.addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    do {
        DummyEvent::semaphore.wait();
    }while(DummyEvent::getRanCount() < 10);
    CHECK_EQUAL(DummyEvent::getRanCount(), 10);
}
/** 
 * \brief tests the destructor when there are tasks left and start was called.
 */
TEST(EventExecutor, destroyTest){
    std::shared_ptr<Common::EventInterface> event1(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event2(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event3(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event4(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event5(new DummyEvent());

    // New so we can call the destructor easily with delete
    Common::EventExecutor *uut = new Common::EventExecutor();
    CHECK(!uut->isRunning());

    // Pretend start was called.
    uut->m_isRunning = true;

    uut->addEvent(event1);
    uut->addEvent(event2);
    uut->addEvent(event3);
    uut->addEvent(event4);
    uut->addEvent(event5);
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    CHECK_EQUAL(DummyEvent::ranCount, 0);
    delete uut;
    CHECK_EQUAL(DummyEvent::ranCount, 10);
}

/**
 * \brief tests the destructor when there are tasks left, but start was not called.
 */
TEST(EventExecutor, destroyTestStartNotCalled){
    std::shared_ptr<Common::EventInterface> event1(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event2(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event3(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event4(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event5(new DummyEvent());

    // New so we can call the destructor easily with delete
    Common::EventExecutor *uut = new Common::EventExecutor();
    CHECK(!uut->isRunning());

    uut->addEvent(event1);
    uut->addEvent(event2);
    uut->addEvent(event3);
    uut->addEvent(event4);
    uut->addEvent(event5);
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    CHECK_EQUAL(DummyEvent::ranCount, 0);
    delete uut;

    // Start was not called, count should be 0
    CHECK_EQUAL(DummyEvent::ranCount, 0);
}

#endif
