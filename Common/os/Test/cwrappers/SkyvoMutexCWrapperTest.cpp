#ifndef ASM_JS

#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public

#include "MockSkyvoMutex.h"
#include "cwrappers/SkyvoMutexCWrapper.h"
#include "cwrappers/SkyvoMutexStruct.h"

TEST_GROUP(SkyvoMutexCWrapper){
    TEST_SETUP(){
        m_mutex = new testing::StrictMock<SkyvoOS::MockSkyvoMutex>();
        m_uut = createMutex();
        delete m_uut->m_mutex;
        m_uut->m_mutex = m_mutex;
    }

    TEST_TEARDOWN(){
        deleteMutex(m_uut);
        //m_mutex is deleted in m_uut
    }
    testing::StrictMock<SkyvoOS::MockSkyvoMutex> *m_mutex;
    SkyvoMutex_t *m_uut;
};

TEST(SkyvoMutexCWrapper, lockTest){
    EXPECT_CALL(*m_mutex, lock());
    lockMutex(m_uut);
}

TEST(SkyvoMutexCWrapper, unlockTest){
    EXPECT_CALL(*m_mutex, unlock());
    unlockMutex(m_uut);
}

#endif

