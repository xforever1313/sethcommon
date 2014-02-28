
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <string>

#define private public

#include "cwrappers/SkyvoString.h"
#include "cwrappers/SkyvoStringStruct.h"

std::string *m_string;
SkyvoString_t m_uut("");

TEST_GROUP(SkyvoString){
    
    TEST_SETUP(){
        m_string = new std::string("Hello world!");
        m_uut = createString(m_string->c_str());
    }
    
    TEST_TEARDOWN(){
        delete m_string;
    }
};

TEST(SkyvoString, getCStringTest){
    CHECK_EQUAL(std::string(getCString(m_uut)), *m_string);
}

///\brief ensures that if the original c_string is deleted, it doesnt crash.
TEST(SkyvoString, IndependentPointerTest){
    std::string s(*m_string);
    delete m_string;
    m_string = NULL;
    CHECK_EQUAL(std::string(getCString(m_uut)), s);
}

TEST(SkyvoString, sizeTest){
    CHECK_EQUAL(getSizeOfString(m_uut), m_string->size());
}

TEST(SkyvoString, copyStringTest){
    SkyvoString_t s = copyString(m_uut);
    CHECK(s == m_uut);
    CHECK_EQUAL(std::string(getCString(s)), std::string(getCString(m_uut)));
}

TEST(SkyvoString, independintCopyTest){
    SkyvoString_t *s1 = new SkyvoString_t(m_string->c_str());
    SkyvoString_t *s2 = new SkyvoString_t(*s1);
    delete s1;
    CHECK(*s2 == m_uut); //Should not segfault
    delete s2;
}

TEST(SkyvoString, indexTest){
    for(size_t i = 0; i < m_string->size(); ++i){
        CHECK_EQUAL(getCharAtIndex(m_uut, i), (*m_string)[i]);
    }
}

TEST(SkyvoString, appendTest){
    (*m_string) += 'a';
    appendToString(m_uut, 'a');
    CHECK_EQUAL(std::string(getCString(m_uut)), *m_string);
}
