#ifndef FILE_SYSTEM_H_INCLUDED
#define FILE_SYSTEM_H_INCLUDED

#include <deque>
#include <string>

namespace SkyvoOS{

/**
* \class FileSystem
* \brief This will be our interface to the file system. It wraps all unistd
* functionality we'll need
* \warning Shortcuts, SymLinks, and hardlinks result in undefined behavior
*
* \author Seth Hendrick
*/
class FileSystem
{
    public:

        enum FileStatus{
            FILE_ERROR = -1,
            FILE_EQUAL = 0,
            FILE_NOT_EQUAL = 1
        };

        static const std::string THIS_DIR; ///<Name for the current directory (usually ".")
        static const std::string UP_DIR;  ///<Name for the directory above the current one (usually "..")

        FileSystem();

        virtual ~FileSystem();

        virtual std::string getCWD();

        ///\brief the equivalent of using "touch"
        virtual bool createFile(const std::string filePath);

        ///\brief creates a directory. Can also create them recusively (e.g. passing in "dir1/dir2" will create dir2 inside of dir1)
        virtual bool createDir(const std::string dirPath);

        virtual FileStatus isFile(const std::string filePath);

        virtual FileStatus isDir(const std::string dirPath);

        virtual bool fileExists(const std::string filePath);

        virtual bool dirExists(const std::string dirPath);

        ///\brief copies the original file to the destination path.  No Op (returns true) if both arguments match
        virtual bool copyFile(const std::string originalFile, const std::string destinationPath);

        ///\brief copies the original dir to the destination path.  Returns false if both arguments match
        virtual bool copyDir (const std::string originalDir, const std::string destinationPath);

        ///\brief renames the File.  Also works as a Move File
        virtual bool renameFile(const std::string originalFile, const std::string newFileName);

        virtual bool renameDir(const std::string originalDir, const std::string newDirName);

        virtual bool deleteFile(const std::string filePath);

        virtual bool deleteDir (const std::string dirPath);

        virtual bool listFilesInDir(const std::string dirPath, std::deque<std::string> &fileNamesInDir);

        ///\return FILE_EQUAL if same, FILE_NOT_EQUAL if not same, FILE_ERROR if error
        ///\details Returns based on file contents, not given names
        virtual FileStatus compareFiles (const std::string fileOne, const std::string fileTwo);

        ///\return FILE_EQUAL if same, FILE_NOT_EQUAL if not same, FILE_ERROR if error
        ///\details Returns based on directory contents, not given file names.  However, file and dir names within the directory must match
        virtual FileStatus compareDirs (const std::string fileOne, const std::string fileTwo);

        ///\return "parent/child"
        virtual std::string pathJoin(const std::string parent, const std::string child);

        #ifdef UNIT_TEST
            bool m_failListFilesInDir;
        #endif

};


}
#endif
