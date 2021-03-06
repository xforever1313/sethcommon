
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILESYSTEMINTERFACE_H_INCLUDED
#define FILESYSTEMINTERFACE_H_INCLUDED

#include <deque>
#include <string>

namespace OS{
class FileSystemInterface{
    public:
        enum FileStatus{
            FILE_ERROR = -1,
            FILE_EQUAL = 0,
            FILE_NOT_EQUAL = 1
        };

        virtual ~FileSystemInterface(){}

        virtual std::string getCWD() = 0;
        virtual bool readFile(std::string &buffer, const std::string &fileName) = 0;
        virtual bool writeFile(const std::string &stringToWrite, const std::string &fileName) = 0;
        virtual bool createFile(const std::string &filePath) = 0;
        virtual bool createDir(const std::string &dirPath) = 0;
        virtual FileStatus isFile(const std::string &filePath) = 0;
        virtual FileStatus isDir(const std::string &dirPath) = 0;
        virtual bool fileExists(const std::string &filePath) = 0;
        virtual bool dirExists(const std::string &dirPath) = 0;
        virtual bool copyFile(const std::string &originalFile, const std::string &destinationPath) = 0;
        virtual bool copyDir (const std::string &originalDir, const std::string &destinationPath) = 0;
        virtual bool renameFile(const std::string &originalFile, const std::string &newFileName) = 0;
        virtual bool renameDir(const std::string &originalDir, const std::string &newDirName) = 0;
        virtual bool deleteFile(const std::string &filePath) = 0;
        virtual bool deleteDir (const std::string &dirPath) = 0;
        virtual bool listFilesInDir(const std::string &dirPath, std::deque<std::string> &fileNamesInDir) = 0;
        virtual FileStatus compareFiles (const std::string &fileOne, const std::string &fileTwo) = 0;
        virtual FileStatus compareDirs (const std::string &dirOne, const std::string &dirTwo) = 0;
};
}

#endif // FILESYSTEMINTERFACE_H_INCLUDED
