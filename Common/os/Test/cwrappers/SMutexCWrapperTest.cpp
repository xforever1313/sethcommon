
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>

#define private public

#include "MockSMutex.h"
#include "cwrappers/SMutexCWrapper.h"
#include "cwrappers/SMutexStruct.h"

TEST_GROUP(SMutexCWrapper){
    TEST_SETUP(){
        m_mutex = new testing::StrictMock<OS::MockSMutex>();
        m_uut = createMutex();
        delete m_uut->m_mutex;
        m_uut->m_mutex = m_mutex;
    }

    TEST_TEARDOWN(){
        deleteMutex(m_uut);
        //m_mutex is deleted in m_uut
    }
    testing::StrictMock<OS::MockSMutex> *m_mutex;
    SMutex_t *m_uut;
};

TEST(SMutexCWrapper, lockTest){
    EXPECT_CALL(*m_mutex, lock());
    lockMutex(m_uut);
}

TEST(SMutexCWrapper, unlockTest){
    EXPECT_CALL(*m_mutex, unlock());
    unlockMutex(m_uut);
}

#endif
