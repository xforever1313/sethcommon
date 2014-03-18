
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <sstream>
#include "Date.h"
#include "TestHelper.h"

OS::Date testTimeStamp;

std::string fileTestOutputPath = "../../../../testOutput/fileSystemTest/fileTest-" + testTimeStamp.getFullDateUnderscores();

std::string path_join(std::string fileName){
    std::stringstream ss;
    ss << testFileDirectory << "/" << fileName;
    return ss.str();
}
