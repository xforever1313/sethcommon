#include <boost/test/unit_test.hpp>

#define private public

#include "DummyEvent.h"
#include "EventExecutor.h"

///\brief tests the run method
BOOST_AUTO_TEST_CASE(EventExecutor_runTest){
    DummyEvent::ranCount = 0;
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
    while (DummyEvent::getRanCount() < 10){
        //Wait for all events to execute
    }
    BOOST_CHECK_EQUAL(DummyEvent::ranCount, 10);
    delete uut;
}

///\brief tests the destructor when there are tasks left
BOOST_AUTO_TEST_CASE(EventExecutor_destroyTest){
    DummyEvent::ranCount = 0;
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
    BOOST_CHECK_EQUAL(DummyEvent::ranCount, 0);
    delete uut;
    BOOST_CHECK_EQUAL(DummyEvent::ranCount, 10);
}
