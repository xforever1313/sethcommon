
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "FilenameCheckerException.h"

TEST_GROUP(FilenameCheckerException){
    TEST_SETUP(){
        m_errorString = "Seth is cool!";
        m_uut = new Common::FilenameCheckerException(m_errorString);
    }
    
    TEST_TEARDOWN(){
        delete m_uut;
    }
    
    std::string m_errorString;
    Common::FilenameCheckerException *m_uut;
};

///\brief Tests the what method
TEST(FilenameCheckerException, whatTest){
    CHECK_EQUAL(m_errorString, m_uut->what());
}
