
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCKFILESYSTEM_H_INCLUDED
#define MOCKFILESYSTEM_H_INCLUDED

#include <deque>
#include <string>

#include "FileSystemInterface.h"
#include "gmock/gmock.h"

class MockFilesystem : public OS::FileSystemInterface{

    public:
        virtual ~MockFilesystem(){};

        MOCK_METHOD0(getCWD, std::string());
        MOCK_METHOD2(readFile, bool(std::string &buffer, const std::string &fileName));
        MOCK_METHOD2(writeFile, bool(const std::string &stringToWrite, const std::string &fileName));
        MOCK_METHOD1(createFile, bool(const std::string &filePath));
        MOCK_METHOD1(createDir, bool(const std::string &dirPath));
        MOCK_METHOD1(isFile, OS::FileSystemInterface::FileStatus(const std::string &filePath));
        MOCK_METHOD1(isDir, OS::FileSystemInterface::FileStatus(const std::string &dirPath));
        MOCK_METHOD1(fileExists, bool(const std::string &filePath));
        MOCK_METHOD1(dirExists, bool(const std::string &dirPath));
        MOCK_METHOD2(copyFile, bool(const std::string &originalFile, const std::string &destinationPath));
        MOCK_METHOD2(copyDir, bool(const std::string &originalDir, const std::string &destinationPath));
        MOCK_METHOD2(renameFile, bool(const std::string &originalFile, const std::string &newFileName));
        MOCK_METHOD2(renameDir, bool(const std::string &originalDir, const std::string &newDirName));
        MOCK_METHOD1(deleteFile, bool(const std::string &filePath));
        MOCK_METHOD1(deleteDir, bool(const std::string &dirPath));
        MOCK_METHOD2(listFilesInDir, bool(const std::string &dirPath, std::deque<std::string> &fileNamesInDir));
        MOCK_METHOD2(compareFiles, OS::FileSystemInterface::FileStatus(const std::string &fileOne, const std::string &fileTwo));
        MOCK_METHOD2(compareDirs, OS::FileSystemInterface::FileStatus(const std::string &dirOne, const std::string &dirTwo));
};

#endif // MOCKFILESYSTEM_H_INCLUDED
