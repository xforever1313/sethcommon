#define private public
#define protected public

#include <boost/test/unit_test.hpp>

#include "FilenameCheckerException.h"

struct FilenameCheckerExceptionFixture{
    FilenameCheckerExceptionFixture() :
        m_errorString("Seth is cool!"),
        m_uut(new Common::FilenameCheckerException(m_errorString))
    {
    }

    virtual ~FilenameCheckerExceptionFixture(){
        delete m_uut;
    }
    std::string m_errorString;
    Common::FilenameCheckerException *m_uut;
};

BOOST_FIXTURE_TEST_SUITE(FilenameCheckerExceptionTest, FilenameCheckerExceptionFixture)

///\brief Tests the what method
BOOST_AUTO_TEST_CASE(FilenameCheckerException_whatTest){
    BOOST_CHECK_EQUAL(m_errorString, m_uut->what());
}

BOOST_AUTO_TEST_SUITE_END()
