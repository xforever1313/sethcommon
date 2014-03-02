
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <string>

#define private public

#include "cwrappers/CString.h"
#include "cwrappers/CStringStruct.h"

std::string *m_string;
CString_t m_uut("");

TEST_GROUP(CString){
    
    TEST_SETUP(){
        m_string = new std::string("Hello world!");
        m_uut = createString(m_string->c_str());
    }
    
    TEST_TEARDOWN(){
        delete m_string;
    }
};

TEST(CString, getCStringTest){
    CHECK_EQUAL(std::string(getCString(m_uut)), *m_string);
}

///\brief ensures that if the original c_string is deleted, it doesnt crash.
TEST(CString, IndependentPointerTest){
    std::string s(*m_string);
    delete m_string;
    m_string = NULL;
    CHECK_EQUAL(std::string(getCString(m_uut)), s);
}

TEST(CString, sizeTest){
    CHECK_EQUAL(getSizeOfString(m_uut), m_string->size());
}

TEST(CString, copyStringTest){
    CString_t s = copyString(m_uut);
    CHECK(s == m_uut);
    CHECK_EQUAL(std::string(getCString(s)), std::string(getCString(m_uut)));
}

TEST(CString, independintCopyTest){
    CString_t *s1 = new CString_t(m_string->c_str());
    CString_t *s2 = new CString_t(*s1);
    delete s1;
    CHECK(*s2 == m_uut); //Should not segfault
    delete s2;
}

TEST(CString, indexTest){
    for(size_t i = 0; i < m_string->size(); ++i){
        CHECK_EQUAL(getCharAtIndex(m_uut, i), (*m_string)[i]);
    }
}

TEST(CString, appendTest){
    (*m_string) += 'a';
    appendToString(m_uut, 'a');
    CHECK_EQUAL(std::string(getCString(m_uut)), *m_string);
}
