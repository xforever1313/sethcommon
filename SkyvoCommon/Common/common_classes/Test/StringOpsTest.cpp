#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <deque>
#include <list>
#include <string>
#include <vector>

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
        m_orderedVector = {"!@#$", "123", "al, Alpha", "Beta", "boy", "Cat", "cool", "Dog", "dog", "ever", "EVERYWHERE", "fREd", "Friend",
                          "good", "gOOF", "Hi", "Ho", "Ice cream", "ice CREAM", "Jr.", "Junior Burger", "Kite", "kYlE", "Marsh", "milk", "No", "Not",
                          "Oh Dear", "Oh no!", "Peas", "Peppers", "Queen", "queen", "R", "r", "Seth", "Seth Hendrick", "TV", "tx queue", "U", "u",
                          "Victor", "Victory", "Walk", "waLker", "X-ray", "xray", "You", "You THERE!", "Zoo", "zoo"};

        m_orderedDeque = std::deque<std::string>(m_orderedVector.begin(), m_orderedVector.end());
        m_orderedList = std::list<std::string>(m_orderedVector.begin(), m_orderedVector.end());
    }

    virtual ~StringOpsFixture(){
    }

    template <typename T>
    static bool compareCollections(const T &c1, const T &c2){
        bool same = true;
        if (c1.size() != c2.size()){
            same = false;
        }
        else{
            for (typename T::const_iterator i = c1.begin(), j = c2.begin(); (i != c1.end()) && (j != c2.end()); ++i, ++j){
                if ((*i) != (*j)){
                    same = false;
                }
            }
        }
        return same;
    }

    std::string m_lowerCaseString;
    std::string m_upperCaseString;
    std::string m_whiteSpacedString;
    std::string m_removedWhiteSpaceString;
    std::string m_tabbedString;
    std::string m_removedTabbedString;
    std::vector<std::string> m_orderedVector;
    std::deque <std::string> m_orderedDeque;
    std::list <std::string> m_orderedList;
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

BOOST_AUTO_TEST_CASE(StringOps_toAlphabeticalOrderModifyVectorTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedVector);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::vector<std::string> >(unorderedVector, m_orderedVector));
    }
}

BOOST_AUTO_TEST_CASE(StringOps_toAlphabeticalOrderCopyVectorTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());

        std::vector<std::string> oldVector(unorderedVector);
        std::vector<std::string> newVector = Common::StringOps::toAlphabeticalOrderCopy(unorderedVector);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::vector<std::string> >(newVector, m_orderedVector));
        BOOST_CHECK(StringOpsFixture::compareCollections<std::vector<std::string> >(oldVector, unorderedVector));
    }
}

BOOST_AUTO_TEST_CASE(StringOps_toAlphabeticalOrderModifyDequeTest){
    for (size_t i = 0; i < 100; ++i){
        std::deque<std::string> unorderedDeque(m_orderedDeque);
        std::random_shuffle(unorderedDeque.begin(), unorderedDeque.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedDeque);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::deque<std::string> >(unorderedDeque, m_orderedDeque));
    }
}

BOOST_AUTO_TEST_CASE(StringOps_toAlphabeticalOrderCopyDequeTest){
    for (size_t i = 0; i < 100; ++i){
        std::deque<std::string> unorderedDeque(m_orderedDeque);
        std::random_shuffle(unorderedDeque.begin(), unorderedDeque.end());

        std::deque<std::string> oldDeque(unorderedDeque);
        std::deque<std::string> newDeque = Common::StringOps::toAlphabeticalOrderCopy(unorderedDeque);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::deque<std::string> >(newDeque, m_orderedDeque));
        BOOST_CHECK(StringOpsFixture::compareCollections<std::deque<std::string> >(oldDeque, unorderedDeque));
    }
}

BOOST_AUTO_TEST_CASE(StringOps_toAlphabeticalOrderModifyListTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());
        std::list <std::string> unorderedList(unorderedVector.begin(), unorderedVector.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedList);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::list<std::string> >(unorderedList, m_orderedList));
    }
}

BOOST_AUTO_TEST_CASE(StringOps_toAlphabeticalOrderCopyListTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());
        std::list <std::string> unorderedList(unorderedVector.begin(), unorderedVector.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedList);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::list<std::string> >(unorderedList, m_orderedList));;

        std::list<std::string> oldList(unorderedList);
        std::list<std::string> newList = Common::StringOps::toAlphabeticalOrderCopy(unorderedList);
        BOOST_CHECK(StringOpsFixture::compareCollections<std::list<std::string> >(newList, m_orderedList));
        BOOST_CHECK(StringOpsFixture::compareCollections<std::list<std::string> >(oldList, unorderedList));
    }
}

BOOST_AUTO_TEST_SUITE_END()
