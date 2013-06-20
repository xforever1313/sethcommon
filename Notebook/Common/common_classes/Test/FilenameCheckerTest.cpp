#define private public
#define protected public

#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "FilenameChecker.h"
#include "FilenameCheckerException.h"

BOOST_AUTO_TEST_SUITE(FilenameCheckerTest)

BOOST_AUTO_TEST_CASE(FilenameChecker_escapeCharacterCheck){
    BOOST_CHECK_EQUAL(Common::FilenameChecker::ESCAPE_CHARACTER, "+");
}

BOOST_AUTO_TEST_CASE(FilenameChecker_longCharacterCheck){
    std::stringstream ss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE + 1; ++i){
        ss << "i";
    }
    std::string longString = ss.str();
    BOOST_CHECK_THROW(Common::FilenameChecker::checkFilename(longString), Common::FilenameCheckerException);
}

///\brief tests the check file name method
BOOST_AUTO_TEST_CASE(FilenameChecker_checkFilenameTest){
    std::string originalString = "\\/:*&?\"\'<>|^+ .";
    //Expected string
    std::stringstream ss;
    ss << Common::FilenameChecker::BACK_SLASH_REPLACE << Common::FilenameChecker::FRONT_SLASH_REPLACE;
    ss << Common::FilenameChecker::COLON_REPLACE << Common::FilenameChecker::ASTERIC_REPLACE;
    ss << Common::FilenameChecker::AMP_REPLACE << Common::FilenameChecker::QUESTION_REPLACE;
    ss << Common::FilenameChecker::QUOTE_REPLACE << Common::FilenameChecker::APOS_REPLACE;
    ss << Common::FilenameChecker::LT_REPLACE << Common::FilenameChecker::GT_REPLACE;
    ss << Common::FilenameChecker::PIPE_REPLACE << Common::FilenameChecker::CARROT_REPLACE;
    ss << Common::FilenameChecker::PLUS_REPLACE << Common::FilenameChecker::SPACE_REPLACE;
    ss << Common::FilenameChecker::PERIOD_REPLACE;

    std::string newString = Common::FilenameChecker::checkFilename(originalString);
    BOOST_CHECK_EQUAL(newString, ss.str());
}

///\brief tests to make sure replace characters are correct
BOOST_AUTO_TEST_CASE(FilenameChecker_replacementCharacterTest){
    std::vector <std::string> forbiddenChars;
    forbiddenChars.push_back(Common::FilenameChecker::BACK_SLASH_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::FRONT_SLASH_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::COLON_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::ASTERIC_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::AMP_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::QUESTION_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::QUOTE_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::APOS_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::LT_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::GT_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::PIPE_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::CARROT_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::PLUS_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::SPACE_REPLACE);
    forbiddenChars.push_back(Common::FilenameChecker::PERIOD_REPLACE);

    for (unsigned int i = 0; i < forbiddenChars.size(); ++i){
        //Ensure the size of each replacement string is 3
        BOOST_CHECK_EQUAL(forbiddenChars[i].size(), 2);
        //Ensure the beginning and end chars are escape chars
        BOOST_CHECK_EQUAL(forbiddenChars[i][0], Common::FilenameChecker::ESCAPE_CHARACTER[0]);
        for (unsigned int j = 0; j < forbiddenChars.size(); ++j){
            if (i != j){
                //Ensure each replacement character is unique.
                BOOST_CHECK(forbiddenChars[i] != forbiddenChars[j]);
            }
        }
    }
}

///\brief tests the case where there are just under the max characters passed in for non-forbidden characters
BOOST_AUTO_TEST_CASE(FilenameChecker_lessThanMaxCharactersNonForbidden){
    std::stringstream ss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE - 1; ++i){
        ss << "i";
    }
    std::string testReturn;
    BOOST_CHECK_NO_THROW(testReturn = Common::FilenameChecker::checkFilename(ss.str()));
    BOOST_CHECK_EQUAL(testReturn, ss.str());
}

///\brief tests the case where there are just under the max characters passed in for all forbidden characters
BOOST_AUTO_TEST_CASE(FilenameChecker_lessThanMaxCharactersForbidden){
    std::stringstream orignalss;
    std::stringstream expectedss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE - 1; ++i){
        orignalss << Common::FilenameChecker::QUESTION;
        expectedss << Common::FilenameChecker::QUESTION_REPLACE;
    }
    std::string testReturn;
    BOOST_CHECK_NO_THROW(testReturn = Common::FilenameChecker::checkFilename(orignalss.str()));
    BOOST_CHECK_EQUAL(testReturn, expectedss.str());
}

///\brief tests the case where there are equal to the max characters passed in for non-forbidden characters
BOOST_AUTO_TEST_CASE(FilenameChecker_equalToMaxCharactersNonForbidden){
    std::stringstream ss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE ; ++i){
        ss << "i";
    }
    std::string testReturn;
    BOOST_CHECK_NO_THROW(testReturn = Common::FilenameChecker::checkFilename(ss.str()));
    BOOST_CHECK_EQUAL(testReturn, ss.str());
}

///\brief tests the case where there are equal to the max characters passed in for all forbidden characters
BOOST_AUTO_TEST_CASE(FilenameChecker_equalToMaxCharactersForbidden){
    std::stringstream orignalss;
    std::stringstream expectedss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE; ++i){
        orignalss << Common::FilenameChecker::QUESTION;
        expectedss << Common::FilenameChecker::QUESTION_REPLACE;
    }
    std::string testReturn;
    BOOST_CHECK_NO_THROW(testReturn = Common::FilenameChecker::checkFilename(orignalss.str()));
    BOOST_CHECK_EQUAL(testReturn, expectedss.str());
}

///\brief tests the case where there are greater than the max characters passed in for non-forbidden characters
BOOST_AUTO_TEST_CASE(FilenameChecker_greaterThanMaxCharactersNonForbidden){
    std::stringstream orignalss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE + 1; ++i){
        orignalss << "i";
    }

    std::stringstream expectedss;
    expectedss << "Filename " << orignalss.str() << " is bigger than " << Common::FilenameChecker::MAX_FILE_SIZE << " characters";

    try{
        Common::FilenameChecker::checkFilename(orignalss.str());
    }
    catch(const Common::FilenameCheckerException &e){
        BOOST_CHECK_EQUAL(e.what(), expectedss.str());
    }
}

///\brief tests the case where there are greater than the max characters passed in for all forbidden characters
BOOST_AUTO_TEST_CASE(FilenameChecker_greaterThanMaxCharactersForbidden){
    std::stringstream orignalss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE + 1; ++i){
        orignalss << Common::FilenameChecker::QUESTION;
    }

    std::stringstream expectedss;
    expectedss << "Filename " << orignalss.str() << " is bigger than " << Common::FilenameChecker::MAX_FILE_SIZE << " characters";

    try{
        Common::FilenameChecker::checkFilename(orignalss.str());
    }
    catch(const Common::FilenameCheckerException &e){
        BOOST_CHECK_EQUAL(e.what(), expectedss.str());
    }
}

///\brief tests to make sure capital letters go to lowercase
BOOST_AUTO_TEST_CASE(FilenameChecker_capitalLetterTest){
    std::string original = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string expected = "abcdefghijklmnopqrstuvwxyz1234567890";

    std::string testReturn = Common::FilenameChecker::checkFilename(original);

    BOOST_CHECK_EQUAL(testReturn, expected);
}

BOOST_AUTO_TEST_SUITE_END()
