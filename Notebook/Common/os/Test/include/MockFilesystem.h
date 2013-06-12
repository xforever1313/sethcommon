#ifndef MOCKFILESYSTEM_H_INCLUDED
#define MOCKFILESYSTEM_H_INCLUDED

#include <deque>
#include <string>

#include "FileSystem.h"
#include "gmock/gmock.h"

class MockFilesystem : public SkyvoOS::FileSystem{

    public:
        MockFilesystem() :
            SkyvoOS::FileSystem(){}
        virtual ~MockFilesystem(){};

        MOCK_METHOD0(getCWD, std::string());
        MOCK_METHOD1(createFile, bool(const std::string filePath));
        MOCK_METHOD1(createDir, bool(std::string createDir));
        MOCK_METHOD1(isFile, SkyvoOS::FileSystem::FileStatus(const std::string filePath));
        MOCK_METHOD1(isDir, SkyvoOS::FileSystem::FileStatus(const std::string dirPath));
        MOCK_METHOD1(fileExists, bool(std::string filePath));
        MOCK_METHOD1(dirExists, bool(const std::string dirPath));
        MOCK_METHOD2(copyFIle, bool(const std::string originalFile, const std::string destinationPath));
        MOCK_METHOD2(copyDir, bool(const std::string originalDir, const std::string destinationPath));
        MOCK_METHOD2(renameFile, bool(const std::string originalFile, std::string newFileName));
        MOCK_METHOD2(renameDir, bool(const std::string originalDir, const std::string newDirName));
        MOCK_METHOD1(deleteFile, bool(const std::string filePath));
        MOCK_METHOD1(deleteDir, bool(const std::string dirPath));
        MOCK_METHOD2(listFilesInDir, bool(const std::string dirPath, std::deque<std::string> &fileNamesInDir));
        MOCK_METHOD2(compareFiles, SkyvoOS::FileSystem::FileStatus(const std::string fileOne, const std::string fileTwo));
        MOCK_METHOD2(compareDirs, SkyvoOS::FileSystem::FileStatus(const std::string fileOne, const std::string fileTwo));
};

#endif // MOCKFILESYSTEM_H_INCLUDED
