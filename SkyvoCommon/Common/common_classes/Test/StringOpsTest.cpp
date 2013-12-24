#include <algorithm>
#include <CppUTest/TestHarness.h>
#include <deque>
#include <list>
#include <string>
#include <vector>

#define private public

#include "StringOps.h"

TEST_GROUP(StringOps){
    TEST_SETUP(){
        m_lowerCaseString = ("abcdefghijklmnopqrstuvwxyz1234567890");
        m_upperCaseString = ("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
        m_whiteSpacedString = (" \t\vH \fe\nl\t \n l\ro\n\t ");
        m_removedWhiteSpaceString = ("Hello");
        m_tabbedString = ("\tHello\tWorld\tHi\t\t");
        m_removedTabbedString = ("    Hello    World    Hi        ");
        m_orderedVector = {"!@#$", "123", "al, Alpha", "Beta", "boy", "Cat", "cool", "Dog", "dog", "ever", "EVERYWHERE", "fREd", "Friend",
                          "good", "gOOF", "Hi", "Ho", "Ice cream", "ice CREAM", "Jr.", "Junior Burger", "Kite", "kYlE", "Marsh", "milk", "No", "Not",
                          "Oh Dear", "Oh no!", "Peas", "Peppers", "Queen", "queen", "R", "r", "Seth", "Seth Hendrick", "TV", "tx queue", "U", "u",
                          "Victor", "Victory", "Walk", "waLker", "X-ray", "xray", "You", "You THERE!", "Zoo", "zoo"};

        m_orderedDeque = std::deque<std::string>(m_orderedVector.begin(), m_orderedVector.end());
        m_orderedList = std::list<std::string>(m_orderedVector.begin(), m_orderedVector.end());
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

TEST(StringOps, toLowerCopyTestWithUpperString){
    std::string oldString(m_upperCaseString);
    std::string newString = Common::StringOps::toLowerCaseCopy(m_upperCaseString);
    CHECK_EQUAL(oldString, m_upperCaseString);
    CHECK_EQUAL(newString, m_lowerCaseString);
}

TEST(StringOps, stoLowerCopyTestWithLowerString){
    std::string oldString(m_lowerCaseString);
    std::string newString = Common::StringOps::toLowerCaseCopy(m_lowerCaseString);
    CHECK_EQUAL(oldString, m_lowerCaseString);
    CHECK_EQUAL(newString, m_lowerCaseString);
}

TEST(StringOps, toLowerModifyTestWithUpperString){
    Common::StringOps::toLowerCaseModify(m_upperCaseString);
    CHECK_EQUAL(m_lowerCaseString, m_upperCaseString);
}

TEST(StringOps, toLowerModifyTestWithLowerString){
    std::string oldString(m_lowerCaseString);
    Common::StringOps::toLowerCaseModify(m_lowerCaseString);
    CHECK_EQUAL(oldString, m_lowerCaseString);
}

TEST(StringOps, toUpperCopyTestWithLowerString){
    std::string oldString(m_lowerCaseString);
    std::string newString = Common::StringOps::toUpperCaseCopy(m_lowerCaseString);
    CHECK_EQUAL(oldString, m_lowerCaseString);
    CHECK_EQUAL(newString, m_upperCaseString);
}

TEST(StringOps, toUpperCopyTestWithUpperString){
    std::string oldString(m_upperCaseString);
    std::string newString = Common::StringOps::toUpperCaseCopy(m_upperCaseString);
    CHECK_EQUAL(oldString, m_upperCaseString);
    CHECK_EQUAL(newString, m_upperCaseString);
}

TEST(StringOps, toUpperModifyTestWithLowerString){
    Common::StringOps::toUpperCaseModify(m_lowerCaseString);
    CHECK_EQUAL(m_lowerCaseString, m_upperCaseString);
}

TEST(StringOps, toUpperModifyTestWithUpperString){
    std::string oldString(m_upperCaseString);
    Common::StringOps::toUpperCaseModify(m_upperCaseString);
    CHECK_EQUAL(oldString, m_upperCaseString);
}

TEST(StringOps, removeWhiteSpaceModifyTest){
    Common::StringOps::removeWhiteSpaceModify(m_whiteSpacedString);
    CHECK_EQUAL(m_whiteSpacedString, m_removedWhiteSpaceString);
}

TEST(StringOps, removeWhiteSpaceCopyTest){
    std::string oldString(m_whiteSpacedString);

    std::string newString = Common::StringOps::removeWhiteSpaceCopy(m_whiteSpacedString);

    CHECK_EQUAL(oldString, m_whiteSpacedString);
    CHECK_EQUAL(newString, m_removedWhiteSpaceString);
}

TEST(StringOps, replaceTabsWithSpacesModifyTest){
    Common::StringOps::replaceTabsWithSpacesModify(m_tabbedString);
    CHECK_EQUAL(m_tabbedString, m_removedTabbedString);
}

TEST(StringOps, replaceTabsWithSpacesCopyTest){
    std::string oldString(m_tabbedString);

    std::string newString = Common::StringOps::replaceTabsWithSpacesCopy(m_tabbedString);

    CHECK_EQUAL(m_tabbedString, oldString);
    CHECK_EQUAL(m_removedTabbedString, newString);
}

TEST(StringOps, toAlphabeticalOrderModifyVectorTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedVector);
        CHECK(compareCollections<std::vector<std::string> >(unorderedVector, m_orderedVector));
    }
}

TEST(StringOps, toAlphabeticalOrderCopyVectorTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());

        std::vector<std::string> oldVector(unorderedVector);
        std::vector<std::string> newVector = Common::StringOps::toAlphabeticalOrderCopy(unorderedVector);
        CHECK(compareCollections<std::vector<std::string> >(newVector, m_orderedVector));
        CHECK(compareCollections<std::vector<std::string> >(oldVector, unorderedVector));
    }
}

TEST(StringOps, toAlphabeticalOrderModifyDequeTest){
    for (size_t i = 0; i < 100; ++i){
        std::deque<std::string> unorderedDeque(m_orderedDeque);
        std::random_shuffle(unorderedDeque.begin(), unorderedDeque.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedDeque);
        CHECK(compareCollections<std::deque<std::string> >(unorderedDeque, m_orderedDeque));
    }
}

TEST(StringOps, toAlphabeticalOrderCopyDequeTest){
    for (size_t i = 0; i < 100; ++i){
        std::deque<std::string> unorderedDeque(m_orderedDeque);
        std::random_shuffle(unorderedDeque.begin(), unorderedDeque.end());

        std::deque<std::string> oldDeque(unorderedDeque);
        std::deque<std::string> newDeque = Common::StringOps::toAlphabeticalOrderCopy(unorderedDeque);
        CHECK(compareCollections<std::deque<std::string> >(newDeque, m_orderedDeque));
        CHECK(compareCollections<std::deque<std::string> >(oldDeque, unorderedDeque));
    }
}

TEST(StringOps, toAlphabeticalOrderModifyListTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());
        std::list <std::string> unorderedList(unorderedVector.begin(), unorderedVector.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedList);
        CHECK(compareCollections<std::list<std::string> >(unorderedList, m_orderedList));
    }
}

TEST(StringOps, toAlphabeticalOrderCopyListTest){
    for (size_t i = 0; i < 100; ++i){
        std::vector<std::string> unorderedVector(m_orderedVector);
        std::random_shuffle(unorderedVector.begin(), unorderedVector.end());
        std::list <std::string> unorderedList(unorderedVector.begin(), unorderedVector.end());

        Common::StringOps::toAlphabeticalOrderModify(unorderedList);
        CHECK(compareCollections<std::list<std::string> >(unorderedList, m_orderedList));;

        std::list<std::string> oldList(unorderedList);
        std::list<std::string> newList = Common::StringOps::toAlphabeticalOrderCopy(unorderedList);
        CHECK(compareCollections<std::list<std::string> >(newList, m_orderedList));
        CHECK(compareCollections<std::list<std::string> >(oldList, unorderedList));
    }
}
