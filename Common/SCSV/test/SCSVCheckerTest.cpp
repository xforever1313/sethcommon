
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>
#include <string>
#include <sstream>

#define private public
#define protected public

#include "SCSVChecker.h"
#include "SCSVConstants.h"

TEST_GROUP(SCSVChecker){
};

TEST(SCSVChecker, checkTest){

    std::string testString = "& , &,";
    std::stringstream ss;
    ss << SCSV::SCSVConstants::SCSV_AMP_REPLACE << " " << SCSV::SCSVConstants::SCSV_SEPARATOR_REPLACE << " " << SCSV::SCSVConstants::SCSV_AMP_REPLACE << SCSV::SCSVConstants::SCSV_SEPARATOR_REPLACE;

    CHECK_EQUAL(SCSV::SCSVChecker::checkText(testString), ss.str());
}

