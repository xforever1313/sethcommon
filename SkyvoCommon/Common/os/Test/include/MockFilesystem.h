#ifndef MOCKFILESYSTEM_H_INCLUDED
#define MOCKFILESYSTEM_H_INCLUDED

#include <deque>
#include <string>

#include "FileSystemInterface.h"
#include "gmock/gmock.h"

class MockFilesystem : public SkyvoOS::FileSystemInterface{

    public:
        virtual ~MockFilesystem(){};

        MOCK_METHOD0(getCWD, std::string());
        MOCK_METHOD1(createFile, bool(const std::string filePath));
        MOCK_METHOD1(createDir, bool(std::string createDir));
        MOCK_METHOD1(isFile, SkyvoOS::FileSystemInterface::FileStatus(const std::string filePath));
        MOCK_METHOD1(isDir, SkyvoOS::FileSystemInterface::FileStatus(const std::string dirPath));
        MOCK_METHOD1(fileExists, bool(std::string filePath));
        MOCK_METHOD1(dirExists, bool(const std::string dirPath));
        MOCK_METHOD2(copyFile, bool(const std::string originalFile, const std::string destinationPath));
        MOCK_METHOD2(copyDir, bool(const std::string originalDir, const std::string destinationPath));
        MOCK_METHOD2(renameFile, bool(const std::string originalFile, std::string newFileName));
        MOCK_METHOD2(renameDir, bool(const std::string originalDir, const std::string newDirName));
        MOCK_METHOD1(deleteFile, bool(const std::string filePath));
        MOCK_METHOD1(deleteDir, bool(const std::string dirPath));
        MOCK_METHOD2(listFilesInDir, bool(const std::string dirPath, std::deque<std::string> &fileNamesInDir));
        MOCK_METHOD2(compareFiles, SkyvoOS::FileSystemInterface::FileStatus(const std::string fileOne, const std::string fileTwo));
        MOCK_METHOD2(compareDirs, SkyvoOS::FileSystemInterface::FileStatus(const std::string fileOne, const std::string fileTwo));
};

#endif // MOCKFILESYSTEM_H_INCLUDED
