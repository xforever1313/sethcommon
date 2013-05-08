#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>

#define private public
#define protected public

#include "SCSVChecker.h"
#include "SCSVConstants.h"

BOOST_AUTO_TEST_CASE(SCSVChecker_checkTest){

    std::string testString = "& , &,";
    std::stringstream ss;
    ss << SCSV_AMP_REPLACE << " " << SCSV_SEPARATOR_REPLACE << " " << SCSV_AMP_REPLACE << SCSV_SEPARATOR_REPLACE;

    BOOST_CHECK_EQUAL(Core::SCSVChecker::checkText(testString), ss.str());
}
