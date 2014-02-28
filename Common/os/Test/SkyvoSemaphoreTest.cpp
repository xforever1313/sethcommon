
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS //Do not test if emscripten, it will not link

#include <CppUTest/TestHarness.h>

#define private public

#include "SemaphorePoster.h"
#include "SemaphoreWaiter.h"
#include "SkyvoSemaphore.h"

TEST_GROUP(Semaphore){
};

///\brief tests the wait method with an inital count set to zero
TEST(Semaphore, waitTestDefaultConstructor){
    SkyvoOS::SkyvoSemaphore *uut = new SkyvoOS::SkyvoSemaphore;
    SemaphorePoster poster(uut);
    CHECK_EQUAL(uut->getSemaphoreCount(), 0); //Zero by default
    poster.start();
    uut->wait();
    CHECK(poster.getPosted()); //If it gets here, the test was successful, as the program didn't hang
    poster.join(); //Wait for thread to exit
    CHECK_EQUAL(uut->getSemaphoreCount(), 0); //Should stay at zero
    delete uut;
}

///\brief tests the wait method with an inital count not set to zero
TEST(Semaphore, waitTest){
    SkyvoOS::SkyvoSemaphore uut(1);
    CHECK_EQUAL(uut.getSemaphoreCount(), 1);
    uut.wait();
    CHECK_EQUAL(uut.getSemaphoreCount(), 0);
    //If it gets here, the test was successful, as the program didn't hang (no blocking should happen when semahpore count greater than zero
}

///\brief tests the tryWait method
TEST(Semaphore, tryWaitTestGreaterThanZero){
    SkyvoOS::SkyvoSemaphore uut(1);
    CHECK_EQUAL(uut.getSemaphoreCount(), 1);
    CHECK(uut.tryWait()); //Should return true, as the count is greater than zero.
    CHECK(!uut.tryWait()); //Should return false, as the count is zero.
    //The program should NOT hang
}

///\brief tests the tryWait method with initial count at zero
TEST(Semaphore, tryWaitTestAtZero){
    SkyvoOS::SkyvoSemaphore uut;
    CHECK_EQUAL(uut.getSemaphoreCount(), 0);
    CHECK(!uut.tryWait()); //Should return false, as the count is zero.
    CHECK(!uut.tryWait()); //Should return false, as the count is zero.
    //The program should NOT hang
}

///\brief tests the timedWait method not At zero
TEST(Semaphore, timedWaitTestNotAtZero){
    SkyvoOS::SkyvoSemaphore *uut = new SkyvoOS::SkyvoSemaphore(1);
    CHECK_EQUAL(uut->getSemaphoreCount(), 1);
    CHECK(uut->timedWait(10));
    CHECK_EQUAL(uut->getSemaphoreCount(), 0);
    CHECK(!uut->timedWait(10));
    CHECK_EQUAL(uut->getSemaphoreCount(), 0);
    delete uut;
}

///\brief tests the timedWait method
TEST(Semaphore, timedWaitTest){
    SkyvoOS::SkyvoSemaphore *uut = new SkyvoOS::SkyvoSemaphore();
    SemaphorePoster poster1(uut);
    poster1.start();
    CHECK(!uut->timedWait(250)); //Timeout before the thing gets posted, should return false
    CHECK(!poster1.getPosted());
    poster1.join();

    SemaphorePoster poster2 (uut);
    poster2.start();
    CHECK(uut->timedWait(600)); //Wait longer than the post time, should return true from getting posted
    poster2.join();

    delete uut;
}

///\brief tests that all wait methods return after a shutdown
TEST(Semaphore, shutdownTest){
    SkyvoOS::SkyvoSemaphore *uut1 = new SkyvoOS::SkyvoSemaphore();
    CHECK(!uut1->isShutdown());
    uut1->shutdown();
    CHECK(uut1->isShutdown());
    uut1->wait(); //thread should not block
    CHECK(uut1->tryWait());
    CHECK(uut1->timedWait(10000));
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
    CHECK(uut2->isShutdown());

    delete uut2;
}

#endif
