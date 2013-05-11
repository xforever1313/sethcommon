#include <string>
#include <sstream>
#include "Date.h"
#include "TestHelper.h"

SkyvoOS::Date testTimeStamp;

std::string fileTestOutputPath = "../../testOutput/fileSystemTest/fileTest-" + testTimeStamp.getFullDateUnderscores();

std::string path_join(std::string fileName){
    std::stringstream ss;
    ss << testFileDirectory << "/" << fileName;
    return ss.str();
}
