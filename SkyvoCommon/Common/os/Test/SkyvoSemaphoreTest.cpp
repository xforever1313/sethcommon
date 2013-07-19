#include <boost/test/unit_test.hpp>

#define private public

#include "SemaphorePoster.h"
#include "SemaphoreWaiter.h"
#include "SkyvoSemaphore.h"

///\brief tests the wait method with an inital count set to zero
BOOST_AUTO_TEST_CASE(Semaphore_waitTestDefaultConstructor){
    SkyvoOS::SkyvoSemaphore *uut = new SkyvoOS::SkyvoSemaphore;
    SemaphorePoster poster(uut);
    poster.start();
    uut->wait();
    BOOST_CHECK(poster.getPosted()); //If it gets here, the test was successful, as the program didn't hang
    poster.join(); //Wait for thread to exit
    delete uut;
}

///\brief tests the wait method with an inital count not set to zero
BOOST_AUTO_TEST_CASE(Semaphore_waitTest){
    SkyvoOS::SkyvoSemaphore uut(1);
    uut.wait();
    //If it gets here, the test was successful, as the program didn't hang (no blocking should happen when semahpore count greater than zero
}


///\brief tests the tryWait method
BOOST_AUTO_TEST_CASE(Semaphore_tryWaitTest){
    SkyvoOS::SkyvoSemaphore uut(1);
    BOOST_CHECK(uut.tryWait()); //Should return true, as the count is greater than zero.
    BOOST_CHECK(!uut.tryWait()); //Should return false, as the count is zero.
    //The program should NOT hang
}

///\brief tests the timedWait method
BOOST_AUTO_TEST_CASE(Semaphore_timedWaitTest){
    SkyvoOS::SkyvoSemaphore *uut = new SkyvoOS::SkyvoSemaphore();
    SemaphorePoster poster1(uut);
    poster1.start();
    BOOST_CHECK(!uut->timedWait(250)); //Timeout before the thing gets posted, should return false
    BOOST_CHECK(!poster1.getPosted());
    poster1.join();

    SemaphorePoster poster2 (uut);
    poster2.start();
    BOOST_CHECK(uut->timedWait(600)); //Wait longer than the post time, should return true from getting posted
    poster2.join();

    delete uut;
}

///\brief tests that all wait methods return after a shutdown
BOOST_AUTO_TEST_CASE(Semaphore_shutdownTest){
    SkyvoOS::SkyvoSemaphore *uut1 = new SkyvoOS::SkyvoSemaphore();
    BOOST_CHECK(!uut1->isShutdown());
    uut1->shutdown();
    BOOST_CHECK(uut1->isShutdown());
    uut1->wait(); //thread should not block
    BOOST_CHECK(uut1->tryWait());
    BOOST_CHECK(uut1->timedWait(10000));
    delete uut1;

    //Test the case where the semaphore is waiting,
    SkyvoOS::SkyvoSemaphore *uut2 = new SkyvoOS::SkyvoSemaphore();
    SemaphoreWaiter waiter1 (uut2);
    waiter1.start();
    SemaphoreWaiter waiter2 (uut2);
    waiter2.start();
    SemaphoreWaiter waiter3 (uut2);
    waiter3.start();
    uut2->shutdown();
    waiter1.join();
    waiter2.join();
    waiter3.join();
    //If the program doesn't hang, test successful!
    BOOST_CHECK(uut2->isShutdown());

    delete uut2;
}
