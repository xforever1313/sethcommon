
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS 

#include <CppUTest/TestHarness.h>

#define private public

#include "DummyEvent.h"
#include "EventExecutor.h"

TEST_GROUP(EventExecutor){
    TEST_SETUP(){
        DummyEvent::ranCount = 0;
    }
};

///\brief tests the run method
TEST(EventExecutor, runTest){
    DummyEvent *event1 = new DummyEvent();
    DummyEvent *event2 = new DummyEvent();
    DummyEvent *event3 = new DummyEvent();
    DummyEvent *event4 = new DummyEvent();
    DummyEvent *event5 = new DummyEvent();
    DummyEvent *event6 = new DummyEvent();
    DummyEvent *event7 = new DummyEvent();
    DummyEvent *event8 = new DummyEvent();
    DummyEvent *event9 = new DummyEvent();
    DummyEvent *event10 = new DummyEvent();
    Common::EventExecutor::startRightAway = true;
    Common::EventExecutor *uut = new Common::EventExecutor();
    uut->addEvent(event1);
    uut->addEvent(event2);
    uut->addEvent(event3);
    uut->addEvent(event4);
    uut->addEvent(event5);
    uut->addEvent(event6);
    uut->addEvent(event7);
    uut->addEvent(event8);
    uut->addEvent(event9);
    uut->addEvent(event10);
    do {
        DummyEvent::semaphore.wait();
    }while(DummyEvent::getRanCount() < 10);
    CHECK_EQUAL(DummyEvent::getRanCount(), 10);
    delete uut;
}

///\brief tests the destructor when there are tasks left
TEST(EventExecutor, destroyTest){
    DummyEvent *event1 = new DummyEvent();
    DummyEvent *event2 = new DummyEvent();
    DummyEvent *event3 = new DummyEvent();
    DummyEvent *event4 = new DummyEvent();
    DummyEvent *event5 = new DummyEvent();
    DummyEvent *event6 = new DummyEvent();
    DummyEvent *event7 = new DummyEvent();
    DummyEvent *event8 = new DummyEvent();
    DummyEvent *event9 = new DummyEvent();
    DummyEvent *event10 = new DummyEvent();
    Common::EventExecutor::startRightAway = false;
    Common::EventExecutor *uut = new Common::EventExecutor();
    uut->addEvent(event1);
    uut->addEvent(event2);
    uut->addEvent(event3);
    uut->addEvent(event4);
    uut->addEvent(event5);
    uut->addEvent(event6);
    uut->addEvent(event7);
    uut->addEvent(event8);
    uut->addEvent(event9);
    uut->addEvent(event10);
    CHECK_EQUAL(DummyEvent::ranCount, 0);
    delete uut;
    CHECK_EQUAL(DummyEvent::ranCount, 10);
}

#endif

