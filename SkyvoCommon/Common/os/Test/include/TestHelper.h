#ifndef TESTHELPER_H_INCLUDED
#define TESTHELPER_H_INCLUDED

/**
* \file TestHelper.h
* \brief includes fucntions that multiple unit tests may need
* \author Seth Hendrick
*/

#include <iostream>
#include <string>

const std::string testFileDirectory = "../../Test"; /**<Location of tests relative to .exe */
const std::string unEditableFilePath = "../../Test/unEditableFile.txt";
const std::string unReadableFilePath = "../../Test/unReadableFile.txt";
const std::string blankFilePath = "../../testOutput/thisFileShouldbeBlank.txt";
const std::string testOutputPath = "../../testOutput";
extern std::string fileTestOutputPath;

/**
 * \brief combines the given string with the testFileLocation
 */
std::string path_join(std::string fileName);

#endif // TESTHELPER_H_INCLUDED