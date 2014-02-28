
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>
#include <string>

#define private public
#define protected public

#include "SCSVException.h"

TEST_GROUP(SCSVException){

    TEST_SETUP(){
        m_error = SCSV::SCSV_OPEN_ERR;
	m_uut = new SCSV::SCSVException(m_error);
    }

    TEST_TEARDOWN(){
        delete m_uut;
    }

    SCSV::SCSVLoadErrors m_error;
    SCSV::SCSVException *m_uut;
};

TEST(SCSVException, getErrorTest){
    CHECK_EQUAL(m_uut->getError(), m_error);
}

TEST(SCSVException, whatTest){
    for (int e = static_cast<int>(SCSV::SCSV_OKAY); e <= SCSV::SCSV_DEFAULT; e += 1){
	SCSV::SCSVException uut(static_cast<SCSV::SCSVLoadErrors>(e));
        STRCMP_EQUAL(SCSV::SCSVException::getErrorMessage(static_cast<SCSV::SCSVLoadErrors>(e)).c_str(), uut.what());
    }
}

