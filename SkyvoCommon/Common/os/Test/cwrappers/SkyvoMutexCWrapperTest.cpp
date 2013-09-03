#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>

#define private public

#include "MockSkyvoMutex.h"
#include "cwrappers/SkyvoMutexCWrapper.h"
#include "cwrappers/SkyvoMutexStruct.h"

struct SkyvoMutexCWrapperFixture{
    SkyvoMutexCWrapperFixture() :
        m_mutex(new testing::StrictMock<SkyvoOS::MockSkyvoMutex>()),
        m_uut(createMutex())
    {
        delete m_uut->m_mutex;
        m_uut->m_mutex = m_mutex;
    }

    virtual ~SkyvoMutexCWrapperFixture(){
        deleteMutex(m_uut);
        //m_mutex is deleted in m_uut
    }
    testing::StrictMock<SkyvoOS::MockSkyvoMutex> *m_mutex;
    SkyvoMutex_t *m_uut;
};

BOOST_FIXTURE_TEST_SUITE(SkyvoMutexCWrapperTest, SkyvoMutexCWrapperFixture)

BOOST_AUTO_TEST_CASE(SkyvoMutexCWrapper_lockTest){
    EXPECT_CALL(*m_mutex, lock());
    lockMutex(m_uut);
}

BOOST_AUTO_TEST_CASE(SkyvoMutexCWrapper_unlockTest){
    EXPECT_CALL(*m_mutex, unlock());
    unlockMutex(m_uut);
}

BOOST_AUTO_TEST_SUITE_END()
