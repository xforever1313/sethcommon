
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include <sstream>
#include <string>
#include <vector>

#include "FilenameChecker.h"
#include "FilenameCheckerException.h"

TEST_GROUP(FilenameChecker){
};

TEST(FilenameChecker, escapeCharacterCheck){
    CHECK_EQUAL(Common::FilenameChecker::ESCAPE_CHARACTER, "+");
}

TEST(FilenameChecker, longCharacterCheck){
    std::stringstream ss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE + 1; ++i){
        ss << "i";
    }
    std::string longString = ss.str();
    try{
        Common::FilenameChecker::checkFilename(longString);
        FAIL("Exception did not throw");
    }
    catch(const Common::FilenameCheckerException &e){
		e.what(); //Override MSVC warning about unreference var
    }
    catch(...){
        FAIL("Incorrect exception thrown");
    }
}

///\brief tests the check file name method
TEST(FilenameChecker, checkFilenameTest){
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
    CHECK_EQUAL(newString, ss.str());
}

///\brief tests to make sure replace characters are correct
TEST(FilenameChecker, replacementCharacterTest){
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
        CHECK_EQUAL(forbiddenChars[i].size(), 2);
        //Ensure the beginning and end chars are escape chars
        CHECK_EQUAL(forbiddenChars[i][0], Common::FilenameChecker::ESCAPE_CHARACTER[0]);
        for (unsigned int j = 0; j < forbiddenChars.size(); ++j){
            if (i != j){
                //Ensure each replacement character is unique.
                CHECK(forbiddenChars[i] != forbiddenChars[j]);
            }
        }
    }
}

///\brief tests the case where there are just under the max characters passed in for non-forbidden characters
TEST(FilenameChecker, lessThanMaxCharactersNonForbidden){
    std::stringstream ss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE - 1; ++i){
        ss << "i";
    }
    std::string testReturn;
    try{
        testReturn = Common::FilenameChecker::checkFilename(ss.str());
    }
    catch(...){
        FAIL("Unexpected Exception");
    }
    CHECK_EQUAL(testReturn, ss.str());
}

///\brief tests the case where there are just under the max characters passed in for all forbidden characters
TEST(FilenameChecker, lessThanMaxCharactersForbidden){
    std::stringstream orignalss;
    std::stringstream expectedss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE - 1; ++i){
        orignalss << Common::FilenameChecker::QUESTION;
        expectedss << Common::FilenameChecker::QUESTION_REPLACE;
    }
    std::string testReturn;
    try{
        testReturn = Common::FilenameChecker::checkFilename(orignalss.str());
    }
    catch(...){
        FAIL("Unexpected Exception");
    }
    CHECK_EQUAL(testReturn, expectedss.str());
}

///\brief tests the case where there are equal to the max characters passed in for non-forbidden characters
TEST(FilenameChecker, equalToMaxCharactersNonForbidden){
    std::stringstream ss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE ; ++i){
        ss << "i";
    }
    std::string testReturn;
    try{
        testReturn = Common::FilenameChecker::checkFilename(ss.str());
    }
    catch(...){
        FAIL("Unexpected Exception");
    }
    CHECK_EQUAL(testReturn, ss.str());
}

///\brief tests the case where there are equal to the max characters passed in for all forbidden characters
TEST(FilenameChecker, equalToMaxCharactersForbidden){
    std::stringstream orignalss;
    std::stringstream expectedss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE; ++i){
        orignalss << Common::FilenameChecker::QUESTION;
        expectedss << Common::FilenameChecker::QUESTION_REPLACE;
    }
    std::string testReturn;
    try{
        testReturn = Common::FilenameChecker::checkFilename(orignalss.str());
    }
    catch(...){
        FAIL("Unexpected Exception");
    }
    CHECK_EQUAL(testReturn, expectedss.str());
}

///\brief tests the case where there are greater than the max characters passed in for non-forbidden characters
TEST(FilenameChecker, greaterThanMaxCharactersNonForbidden){
    std::stringstream orignalss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE + 1; ++i){
        orignalss << "i";
    }

    std::stringstream expectedss;
    expectedss << "Filename " << orignalss.str() << " is bigger than " << Common::FilenameChecker::MAX_FILE_SIZE << " characters";

    try{
        Common::FilenameChecker::checkFilename(orignalss.str());
        FAIL("Exception did not throw");
    }
    catch(const Common::FilenameCheckerException &e){
        CHECK_EQUAL(e.what(), expectedss.str());
    }
}

///\brief tests the case where there are greater than the max characters passed in for all forbidden characters
TEST(FilenameChecker, greaterThanMaxCharactersForbidden){
    std::stringstream orignalss;
    for (unsigned int i = 0; i < Common::FilenameChecker::MAX_FILE_SIZE + 1; ++i){
        orignalss << Common::FilenameChecker::QUESTION;
    }

    std::stringstream expectedss;
    expectedss << "Filename " << orignalss.str() << " is bigger than " << Common::FilenameChecker::MAX_FILE_SIZE << " characters";

    try{
        Common::FilenameChecker::checkFilename(orignalss.str());
        FAIL("Exception did not throw");
    }
    catch(const Common::FilenameCheckerException &e){
        CHECK_EQUAL(e.what(), expectedss.str());
    }
}

///\brief tests to make sure capital letters go to lowercase
TEST(FilenameChecker, capitalLetterTest){
    std::string original = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    std::string expected = "abcdefghijklmnopqrstuvwxyz1234567890";

    std::string testReturn = Common::FilenameChecker::checkFilename(original);

    CHECK_EQUAL(testReturn, expected);
}
