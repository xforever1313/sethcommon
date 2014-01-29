#ifndef FILE_SYSTEM_H_INCLUDED
#define FILE_SYSTEM_H_INCLUDED

#ifdef ASM_JS
    #pragma message("FileSystem not fully supported nor tested with emscripten. Some functions may not work like with C++. Use at own risk.  Good luck!")
#endif

#include <deque>
#include <string>

#include "cstdioWrapperInterface.h"
#include "FileSystemInterface.h"

namespace SkyvoOS{

/**
* \class FileSystem
* \brief This will be our interface to the file system. It wraps all unistd
* functionality we'll need
* \warning Shortcuts, SymLinks, and hardlinks result in undefined behavior
* \warning FileSystem is not fully supported nor has been tested with emscripten.
*          Some functions may not work like with C++.  Use at own risk.  Good Luck!
*
* \author Seth Hendrick
*/
class FileSystem : public FileSystemInterface
{
    public:
        static const std::string THIS_DIR; ///<Name for the current directory (usually ".")
        static const std::string UP_DIR;  ///<Name for the directory above the current one (usually "..")

        ///\return "parent/child"
        static std::string pathJoin(const std::string &parent, const std::string &child);

        ///\brief returns an instance of filesystem.
        ///\warning DO NOT DELETE THE RETURNED POINTER.  It will cause invalid pointers.
        static FileSystemInterface *getInstance();

        virtual ~FileSystem();

        virtual std::string getCWD();

        /**
        * \brief Reads the file, and puts the characters in the passed in buffer string.
        * \note This will NOT clear the buffer, it will append whatever is in the file to the passed in string
        * \return True if no file errors, false if it could not read to file.  The string is NOT cleared in the case of a read error.
        */
        virtual bool readFile(std::string &buffer, const std::string &fileName);

        /**
        * \brief Writes the given string to the given file name.  This OVERWRITES the file.
        * \warning If a write error occurs, the original file contents is lost.
        * \return True if the file was written successfully.  False if the file wasnt opened, or a write error occured.
        */
        virtual bool writeFile(const std::string &stringToWrite, const std::string &fileName);

        ///\brief the equivalent of using "touch"
        virtual bool createFile(const std::string &filePath);

        ///\brief creates a directory. Can also create them recusively (e.g. passing in "dir1/dir2" will create dir2 inside of dir1)
        virtual bool createDir(const std::string &dirPath);

        virtual FileStatus isFile(const std::string &filePath);

        virtual FileStatus isDir(const std::string &dirPath);

        virtual bool fileExists(const std::string &filePath);

        virtual bool dirExists(const std::string &dirPath);

        ///\brief copies the original file to the destination path.  No Op (returns true) if both arguments match
        virtual bool copyFile(const std::string &originalFile, const std::string &destinationPath);

        ///\brief copies the original dir to the destination path.  Returns false if both arguments match
        virtual bool copyDir (const std::string &originalDir, const std::string &destinationPath);

        ///\brief renames the File.  Also works as a Move File
        virtual bool renameFile(const std::string &originalFile, const std::string &newFileName);

        virtual bool renameDir(const std::string &originalDir, const std::string &newDirName);

        virtual bool deleteFile(const std::string &filePath);

        virtual bool deleteDir (const std::string &dirPath);

        virtual bool listFilesInDir(const std::string &dirPath, std::deque<std::string> &fileNamesInDir);

        ///\return FILE_EQUAL if same, FILE_NOT_EQUAL if not same, FILE_ERROR if error
        ///\note Returns based on file contents, not given names
        virtual FileStatus compareFiles (const std::string &fileOne, const std::string &fileTwo);

        ///\return FILE_EQUAL if same, FILE_NOT_EQUAL if not same, FILE_ERROR if error
        ///\note Returns based on directory contents, not given file names.  However, file and dir names within the directory must match
        virtual FileStatus compareDirs (const std::string &dirOne, const std::string &dirTwo);

        #ifdef UNIT_TEST
            bool m_failListFilesInDir;
        #endif

    private:
        FileSystem();
        FileSystem(const FileSystem&);

        cstdioWrapperInterface *m_cstdio;
};

}
#endif
