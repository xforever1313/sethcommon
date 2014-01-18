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
    ss << SCSV_AMP_REPLACE << " " << SCSV_SEPARATOR_REPLACE << " " << SCSV_AMP_REPLACE << SCSV_SEPARATOR_REPLACE;

    CHECK_EQUAL(SCSV::SCSVChecker::checkText(testString), ss.str());
}

