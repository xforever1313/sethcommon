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
        BOOST_CHECK_EQUAL(forbiddenChars[i].size(), 3);
        //Ensure the beginning and end chars are escape chars
        BOOST_CHECK_EQUAL(forbiddenChars[i][0], Common::FilenameChecker::ESCAPE_CHARACTER[0]);
        BOOST_CHECK_EQUAL(forbiddenChars[i][2], Common::FilenameChecker::ESCAPE_CHARACTER[0]);
        for (unsigned int j = 0; j < forbiddenChars.size(); ++j){
            if (i != j){
                //Ensure each replacement character is unique.
                BOOST_CHECK(forbiddenChars[i] != forbiddenChars[j]);
            }
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
