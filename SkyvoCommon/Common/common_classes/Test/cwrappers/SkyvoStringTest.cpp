#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>

#define private public

#include "cwrappers/SkyvoString.h"
#include "cwrappers/SkyvoStringStruct.h"

struct SkyvoStringFixture{
    SkyvoStringFixture() :
        m_string(new std::string("Hello world!")),
        m_uut(createString(m_string->c_str()))
    {
    }
    virtual ~SkyvoStringFixture(){
        delete m_string;
    }

    std::string *m_string;
    SkyvoString_t m_uut;
};

BOOST_FIXTURE_TEST_SUITE(SkyvoStringTest, SkyvoStringFixture)

BOOST_AUTO_TEST_CASE(SkyvoString_getCStringTest){
    BOOST_CHECK_EQUAL(std::string(getCString(m_uut)), *m_string);
}

///\brief ensures that if the original c_string is deleted, it doesnt crash.
BOOST_AUTO_TEST_CASE(SkyvoString_IndependentPointerTest){
    std::string s(*m_string);
    delete m_string;
    m_string = NULL;
    BOOST_CHECK_EQUAL(std::string(getCString(m_uut)), s);
}

BOOST_AUTO_TEST_CASE(SkyvoString_sizeTest){
    BOOST_CHECK_EQUAL(getSizeOfString(m_uut), m_string->size());
}

BOOST_AUTO_TEST_CASE(SkyvoString_copyStringTest){
    SkyvoString_t s = copyString(m_uut);
    BOOST_CHECK(s == m_uut);
    BOOST_CHECK_EQUAL(std::string(getCString(s)), std::string(getCString(m_uut)));
}

BOOST_AUTO_TEST_CASE(SkyvoString_independintCopyTest){
    SkyvoString_t *s1 = new SkyvoString_t(m_string->c_str());
    SkyvoString_t *s2 = new SkyvoString_t(*s1);
    delete s1;
    BOOST_CHECK(*s2 == m_uut); //Should not segfault
    delete s2;
}

BOOST_AUTO_TEST_CASE(SkyvoString_indexTest){
    for(size_t i = 0; i < m_string->size(); ++i){
        BOOST_CHECK_EQUAL(getCharAtIndex(m_uut, i), (*m_string)[i]);
    }
}

BOOST_AUTO_TEST_CASE(SkyvoString_appendTest){
    (*m_string) += 'a';
    appendToString(m_uut, 'a');
    BOOST_CHECK_EQUAL(std::string(getCString(m_uut)), *m_string);
}

BOOST_AUTO_TEST_SUITE_END()
