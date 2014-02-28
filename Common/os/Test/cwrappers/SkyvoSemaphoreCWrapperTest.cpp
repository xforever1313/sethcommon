
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public

#include "MockSkyvoSemaphore.h"
#include "cwrappers/SkyvoBoolean.h"
#include "cwrappers/SkyvoSemaphoreCWrapper.h"
#include "cwrappers/SkyvoSemaphoreStruct.h"

TEST_GROUP(SkyvoSemaphoreCWrapper){
    TEST_SETUP(){
        m_semaphore = new testing::StrictMock<SkyvoOS::MockSkyvoSemaphore>();
        m_uut = createSemaphore();
        CHECK_EQUAL(m_uut->m_initialCount, 0);
        delete m_uut->m_semaphore;
        m_uut->m_semaphore = m_semaphore;
    }

    TEST_TEARDOWN(){
        deleteSemaphore(m_uut);
        //m_semaphore is deleted in m_uut
    }
    testing::StrictMock<SkyvoOS::MockSkyvoSemaphore> *m_semaphore;
    SkyvoSemaphore_t *m_uut;
};

TEST(SkyvoSemaphoreCWrapper, initialCountTest){
    unsigned int count = 10;
    SkyvoSemaphore_t *uut = createSemaphoreWithInitialCount(count);
    CHECK_EQUAL(uut->m_initialCount, count);
    deleteSemaphore(uut);
}

TEST(SkyvoSemaphoreCWrapper, postTest){
    EXPECT_CALL(*m_semaphore, post());
    postToSemaphore(m_uut);
}

TEST(SkyvoSemaphoreCWrapper, waitTest){
    EXPECT_CALL(*m_semaphore, wait());
    waitOnSemaphore(m_uut);
}

TEST(SkyvoSemaphoreCWrapper, tryWaitTestTrue){
    EXPECT_CALL (*m_semaphore, tryWait())
        .WillOnce(testing::Return(true));

    CHECK_EQUAL(tryWaitOnSemaphore(m_uut), TRUE);
}

TEST(SkyvoSemaphoreCWrapper, tryWaitTestFalse){
    EXPECT_CALL (*m_semaphore, tryWait())
        .WillOnce(testing::Return(false));

    CHECK_EQUAL(tryWaitOnSemaphore(m_uut), FALSE);
}

TEST(SkyvoSemaphoreCWrapper, timedWaitTestTrue){
    unsigned long timedWait = 1000;
    EXPECT_CALL (*m_semaphore, timedWait(timedWait))
        .WillOnce(testing::Return(true));

    CHECK_EQUAL(timedWaitOnSemaphore(m_uut, timedWait), TRUE);
}

TEST(SkyvoSemaphoreCWrapper, timedWaitTestFalse){
    unsigned long timedWait = 1000;
    EXPECT_CALL (*m_semaphore, timedWait(timedWait))
        .WillOnce(testing::Return(false));

    CHECK_EQUAL(timedWaitOnSemaphore(m_uut, timedWait), FALSE);
}

TEST(SkyvoSemaphoreCWrapper, shutdownTest){
    EXPECT_CALL(*m_semaphore, shutdown());
    shutdownSemaphore(m_uut);
}

TEST(SkyvoSemaphoreCWrapper, isSemaphoreShutdownTestTrue){
    EXPECT_CALL(*m_semaphore, isShutdown())
        .WillOnce(testing::Return(true));
    CHECK_EQUAL(isSemaphoreShutDown(m_uut), TRUE);
}

TEST(SkyvoSemaphoreCWrapper, isShutdownTestFalse){
    EXPECT_CALL(*m_semaphore, isShutdown())
        .WillOnce(testing::Return(false));
    CHECK_EQUAL(isSemaphoreShutDown(m_uut), FALSE);
}

#endif

