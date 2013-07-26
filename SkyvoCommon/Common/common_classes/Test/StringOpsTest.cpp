#include <boost/test/unit_test.hpp>

#define private public

#include "StringOps.h"

struct StringOpsFixture{
    StringOpsFixture() :
        m_lowerCaseString("abcdefghijklmnopqrstuvwxyz1234567890"),
        m_upperCaseString("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"),
        m_whiteSpacedString(" \t\vH \fe\nl\t \n l\ro\n\t "),
        m_removedWhiteSpaceString("Hello"),
        m_tabbedString("\tHello\tWorld\tHi\t\t"),
        m_removedTabbedString("    Hello    World    Hi        ")
    {
    }

    virtual ~StringOpsFixture(){
    }

    std::string m_lowerCaseString;
    std::string m_upperCaseString;
    std::string m_whiteSpacedString;
    std::string m_removedWhiteSpaceString;
    std::string m_tabbedString;
    std::string m_removedTabbedString;
};

BOOST_FIXTURE_TEST_SUITE(StringOpsTest, StringOpsFixture)

BOOST_AUTO_TEST_CASE(StringOps_toLowerCopyTestWithUpperString){
    std::string oldString(m_upperCaseString);
    std::string newString = Common::StringOps::toLowerCaseCopy(m_upperCaseString);
    BOOST_CHECK_EQUAL(oldString, m_upperCaseString);
    BOOST_CHECK_EQUAL(newString, m_lowerCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toLowerCopyTestWithLowerString){
    std::string oldString(m_lowerCaseString);
    std::string newString = Common::StringOps::toLowerCaseCopy(m_lowerCaseString);
    BOOST_CHECK_EQUAL(oldString, m_lowerCaseString);
    BOOST_CHECK_EQUAL(newString, m_lowerCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toLowerModifyTestWithUpperString){
    Common::StringOps::toLowerCaseModify(m_upperCaseString);
    BOOST_CHECK_EQUAL(m_lowerCaseString, m_upperCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toLowerModifyTestWithLowerString){
    std::string oldString(m_lowerCaseString);
    Common::StringOps::toLowerCaseModify(m_lowerCaseString);
    BOOST_CHECK_EQUAL(oldString, m_lowerCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toUpperCopyTestWithLowerString){
    std::string oldString(m_lowerCaseString);
    std::string newString = Common::StringOps::toUpperCaseCopy(m_lowerCaseString);
    BOOST_CHECK_EQUAL(oldString, m_lowerCaseString);
    BOOST_CHECK_EQUAL(newString, m_upperCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toUpperCopyTestWithUpperString){
    std::string oldString(m_upperCaseString);
    std::string newString = Common::StringOps::toUpperCaseCopy(m_upperCaseString);
    BOOST_CHECK_EQUAL(oldString, m_upperCaseString);
    BOOST_CHECK_EQUAL(newString, m_upperCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toUpperModifyTestWithLowerString){
    Common::StringOps::toUpperCaseModify(m_lowerCaseString);
    BOOST_CHECK_EQUAL(m_lowerCaseString, m_upperCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_toUpperModifyTestWithUpperString){
    std::string oldString(m_upperCaseString);
    Common::StringOps::toUpperCaseModify(m_upperCaseString);
    BOOST_CHECK_EQUAL(oldString, m_upperCaseString);
}

BOOST_AUTO_TEST_CASE(StringOps_removeWhiteSpaceModifyTest){
    Common::StringOps::removeWhiteSpaceModify(m_whiteSpacedString);
    BOOST_CHECK_EQUAL(m_whiteSpacedString, m_removedWhiteSpaceString);
}

BOOST_AUTO_TEST_CASE(StringOps_removeWhiteSpaceCopyTest){
    std::string oldString(m_whiteSpacedString);

    std::string newString = Common::StringOps::removeWhiteSpaceCopy(m_whiteSpacedString);

    BOOST_CHECK_EQUAL(oldString, m_whiteSpacedString);
    BOOST_CHECK_EQUAL(newString, m_removedWhiteSpaceString);
}

BOOST_AUTO_TEST_CASE(StringOps_replaceTabsWithSpacesModifyTest){
    Common::StringOps::replaceTabsWithSpacesModify(m_tabbedString);
    BOOST_CHECK_EQUAL(m_tabbedString, m_removedTabbedString);
}

BOOST_AUTO_TEST_CASE(StringOps_replaceTabsWithSpacesCopyTest){
    std::string oldString(m_tabbedString);

    std::string newString = Common::StringOps::replaceTabsWithSpacesCopy(m_tabbedString);

    BOOST_CHECK_EQUAL(m_tabbedString, oldString);
    BOOST_CHECK_EQUAL(m_removedTabbedString, newString);
}

BOOST_AUTO_TEST_SUITE_END()
