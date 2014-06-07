
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

///\brief tests the run method
TEST(EventExecutor, runTest){
    //Have a mix of shared pointers in different scopes.
    std::shared_ptr<Common::EventInterface> event1(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event2(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event3(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event4(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event5(new DummyEvent());

    Common::EventExecutor::startRightAway = true;
    Common::EventExecutor *uut = new Common::EventExecutor();

    uut->addEvent(event1);
    uut->addEvent(event2);
    uut->addEvent(event3);
    uut->addEvent(event4);
    uut->addEvent(event5);
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<Common::EventInterface> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    uut->addEvent(std::shared_ptr<DummyEvent> (new DummyEvent()));
    do {
        DummyEvent::semaphore.wait();
    }while(DummyEvent::getRanCount() < 10);
    CHECK_EQUAL(DummyEvent::getRanCount(), 10);
    delete uut;
}

///\brief tests the destructor when there are tasks left
TEST(EventExecutor, destroyTest){
    std::shared_ptr<Common::EventInterface> event1(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event2(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event3(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event4(new DummyEvent());
    std::shared_ptr<Common::EventInterface> event5(new DummyEvent());

    Common::EventExecutor::startRightAway = false;
    Common::EventExecutor *uut = new Common::EventExecutor();

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

#endif
