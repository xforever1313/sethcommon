#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>

#define private public

#include "MockSkyvoSemaphore.h"
#include "cwrappers/SkyvoBoolean.h"
#include "cwrappers/SkyvoSemaphoreCWrapper.h"
#include "cwrappers/SkyvoSemaphoreStruct.h"

struct SkyvoSemaphoreCWrapperFixture{
    SkyvoSemaphoreCWrapperFixture() :
        m_semaphore(new testing::StrictMock<SkyvoOS::MockSkyvoSemaphore>()),
        m_uut(createSemaphore())
    {
        BOOST_CHECK_EQUAL(m_uut->m_initialCount, 0);
        delete m_uut->m_semaphore;
        m_uut->m_semaphore = m_semaphore;
    }

    virtual ~SkyvoSemaphoreCWrapperFixture(){
        deleteSemaphore(m_uut);
        //m_semaphore is deleted in m_uut
    }
    testing::StrictMock<SkyvoOS::MockSkyvoSemaphore> *m_semaphore;
    SkyvoSemaphore_t *m_uut;
};

BOOST_FIXTURE_TEST_SUITE(SkyvoSemaphoreCWrapperTest, SkyvoSemaphoreCWrapperFixture)

BOOST_AUTO_TEST_CASE(SkyvoSempahoreCWrapper_initialCountTest){
    unsigned int count = 10;
    SkyvoSemaphore_t *uut = createSemaphoreWithInitialCount(count);
    BOOST_CHECK_EQUAL(uut->m_initialCount, count);
    deleteSemaphore(uut);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_postTest){
    EXPECT_CALL(*m_semaphore, post());
    postToSemaphore(m_uut);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_waitTest){
    EXPECT_CALL(*m_semaphore, wait());
    waitOnSemaphore(m_uut);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_tryWaitTestTrue){
    EXPECT_CALL (*m_semaphore, tryWait())
        .WillOnce(testing::Return(true));

    BOOST_CHECK_EQUAL(tryWaitOnSemaphore(m_uut), TRUE);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_tryWaitTestFalse){
    EXPECT_CALL (*m_semaphore, tryWait())
        .WillOnce(testing::Return(false));

    BOOST_CHECK_EQUAL(tryWaitOnSemaphore(m_uut), FALSE);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_timedWaitTestTrue){
    unsigned long timedWait = 1000;
    EXPECT_CALL (*m_semaphore, timedWait(timedWait))
        .WillOnce(testing::Return(true));

    BOOST_CHECK_EQUAL(timedWaitOnSemaphore(m_uut, timedWait), TRUE);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_timedWaitTestFalse){
    unsigned long timedWait = 1000;
    EXPECT_CALL (*m_semaphore, timedWait(timedWait))
        .WillOnce(testing::Return(false));

    BOOST_CHECK_EQUAL(timedWaitOnSemaphore(m_uut, timedWait), FALSE);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_shutdownTest){
    EXPECT_CALL(*m_semaphore, shutdown());
    shutdownSemaphore(m_uut);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_isSemaphoreShutdownTestTrue){
    EXPECT_CALL(*m_semaphore, isShutdown())
        .WillOnce(testing::Return(true));
    BOOST_CHECK_EQUAL(isSemaphoreShutDown(m_uut), TRUE);
}

BOOST_AUTO_TEST_CASE(SkyvoSemaphoreCWrapper_isShutdownTestFalse){
    EXPECT_CALL(*m_semaphore, isShutdown())
        .WillOnce(testing::Return(false));
    BOOST_CHECK_EQUAL(isSemaphoreShutDown(m_uut), FALSE);
}

BOOST_AUTO_TEST_SUITE_END()
