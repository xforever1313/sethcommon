#include <algorithm>
#include <cstdio>
#include <errno.h>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#ifndef MSVC
	#include <dirent.h>
	#include <unistd.h>
#endif

#ifdef WIN32
	#include <windows.h>
#endif

#include "cstdioWrapper.h"
#include "cstdioWrapperInterface.h"
#include "FileSystem.h"

namespace SkyvoOS{
const std::string FileSystem::THIS_DIR = ".";
const std::string FileSystem::UP_DIR = "..";

const char FileSystem::FILE_SEPARATOR = '/';

FileSystemInterface *FileSystem::getInstance(){
    static FileSystem fs;
    return &fs;
}

std::string FileSystem::pathJoin(const std::string &parent, const std::string &child){
	return std::string(parent + FileSystem::FILE_SEPARATOR + child);
}

FileSystem::FileSystem() :
    m_cstdio(new cstdioWrapper())
{
    #ifdef UNIT_TEST
        m_failListFilesInDir = false;
    #endif
}

FileSystem::~FileSystem(){
    delete m_cstdio;
}

std::string FileSystem::getCWD(){
    std::string ret;
    #ifdef WIN32
    char dir[MAX_PATH];
	GetModuleFileNameA( NULL, dir, MAX_PATH );
    ret = std::string (dir);
    std::string::size_type pos = ret.find_last_of( "\\/" ); //Get rid of the filename
    ret = ret.substr(0, pos);
    
    #else
    
    char dir [PATH_MAX];
    getcwd(dir, PATH_MAX);
    ret = std::string(dir);

    #endif
    return ret;
}

bool FileSystem::readFile(std::string &buffer, const std::string &fileName){
    bool ret = true;
    FILE_t *file = m_cstdio->fopen(fileName, std::string("r"));
    if (file == NULL){
        ret = false;
    }
    else{
        char nextChar;
        while ((nextChar = m_cstdio->fgetc(file)) != cstdioWrapper::END_OF_FILE){
            buffer += nextChar;
        }
        if (m_cstdio->ferror(file) != 0){
            ret = false;
        }
        m_cstdio->fclose(file);
    }
    return ret;
}

bool FileSystem::writeFile(const std::string &stringToWrite, const std::string &fileName){
    bool ret = true;
    FILE_t *file = m_cstdio->fopen(fileName, std::string("w"));
    if (file == NULL){
        ret = false;
    }
    else{

        for (size_t i = 0; (i < stringToWrite.size()) && (m_cstdio->fputc(stringToWrite[i], file) != cstdioWrapper::END_OF_FILE); ++i);
        if (m_cstdio->ferror(file) != 0){
            ret = false;
        }
        m_cstdio->fclose(file);
    }
    return ret;
}

bool FileSystem::createFile(const std::string &filePath){
    bool ret = false;
    FILE_t *file = m_cstdio->fopen(filePath, std::string("w"));
    if (file == NULL){
        ret = false;
    }
    else{
        m_cstdio->fclose(file);
        ret = true;
    }
    return ret;
}

bool FileSystem::createDir(const std::string &dirPath){
    //If path is blank, return false
    if (dirPath == ""){
        return false;
    }
    std::stringstream ss(dirPath);
    std::vector<std::string> dirsToMake;

    char firstChar = ss.peek();
    //If the first character is /, it means we want to create a dir in root.
	if (firstChar == FileSystem::FILE_SEPARATOR){
        std::string nextDir;
		std::getline(ss, nextDir, FileSystem::FILE_SEPARATOR);
        if (ss.str() != ""){                //If empty string, ignore
			nextDir = FileSystem::FILE_SEPARATOR + nextDir;
            dirsToMake.push_back(nextDir);
        }
        ss.peek();
    }
    while (!ss.eof()){
        std::string nextDir;
		std::getline(ss, nextDir, FileSystem::FILE_SEPARATOR);
        dirsToMake.push_back(nextDir);
        ss.peek();
    }

    std::string currentDir = dirsToMake[0];
    for (unsigned int i = 1; i < dirsToMake.size()+1; ++i){
        if (!dirExists(currentDir)){
			#ifdef MSVC
				int status = CreateDirectoryA(currentDir.c_str(), NULL);
				if (status != 0){
					status = 0;
				}
				else{
					status = 1;
				}
			#elif WIN32
				int status = mkdir(currentDir.c_str());
            #else
				mode_t mode = (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); //rwxr-xr-x

				int status = mkdir (currentDir.c_str(), mode);
            #endif
            if (status != 0){
                return false;
            }
        }
        if (i < dirsToMake.size()){
            currentDir = pathJoin(currentDir, dirsToMake[i]);
        }
    }
    return true;
}

bool FileSystem::fileExists(const std::string &filePath){
    bool ret = false;
    FILE_t *file = m_cstdio->fopen(filePath, std::string("r"));
    if (file == NULL){
        ret = false;
    }
    else{
        ret = true;
        m_cstdio->fclose(file);
    }
    return ret;
}

bool FileSystem::dirExists(const std::string &dirPath){
    bool ret = false;

	#ifdef MSVC
	    int status = GetFileAttributesA(dirPath.c_str());
		if (status == FILE_ATTRIBUTE_DIRECTORY){
			ret = true;
		}
	#else
		DIR *dir;
		dir = opendir(dirPath.c_str());
		if (dir == NULL){
			ret = false;
		}
		else{
			ret = true;
		}
		if (dir != NULL){
			closedir(dir);
		}

	#endif
    return ret;
}

FileSystem::FileStatus FileSystem::isFile(const std::string &filePath){
    FileSystem::FileStatus ret = FILE_NOT_EQUAL;

    struct stat s;

    if( stat(filePath.c_str(),&s) == 0 ){
        if( s.st_mode & S_IFREG ){
            //it's a file
            ret = FILE_EQUAL;
        }
        else{
            ret = FILE_NOT_EQUAL;
        }
    }
    else{
        ret = FILE_ERROR;
    }
    return ret;
}

FileSystem::FileStatus FileSystem::isDir(const std::string &dirPath){
    FileSystem::FileStatus ret = FILE_NOT_EQUAL;

    struct stat s;

    if( stat(dirPath.c_str(),&s) == 0 ){
        if( s.st_mode & S_IFDIR ){
            //it's a directory
            ret = FILE_EQUAL;
        }
        else{
            ret = FILE_NOT_EQUAL;
        }
    }
    else{
        ret = FILE_ERROR;
    }
    return ret;
}

bool FileSystem::copyFile(const std::string &originalFile, const std::string &destinationPath){
    bool ret = false;
    if (originalFile == destinationPath){
        ret = true; //No op
    }
    else{
        //If infile fails, return so the outfile isn't whiped out
        std::ifstream inFile(originalFile.c_str());
        if (inFile.fail()){
            ret = false;
        }
        else{
            //Check for blank file
            inFile.peek();
            if (inFile.eof() && !inFile.fail()){
                ret = createFile(destinationPath);
            }
            else{
                std::ofstream outFile(destinationPath.c_str());
                while (!inFile.eof() && !inFile.fail() && !outFile.fail()){
                    outFile.put(inFile.get());
                    inFile.peek();
                }
                if (inFile.fail() || outFile.fail()){
                    ret = false;
                }
                else{
                    ret = true;
                }
            }
        }
    }

    return ret;
}

bool FileSystem::copyDir (const std::string &originalDir, const std::string &destinationPath){
    bool ret = true;
    //Ensure the originalDir actually exists, and the destination does not already exist
    if ((dirExists(originalDir)) && (!dirExists(destinationPath))){
        //Make destination dir
        if (createDir(destinationPath)){
            //Get files listed in originalDir
            std::deque<std::string> files;
            if (listFilesInDir(originalDir, files)){
                for (unsigned int i = 0; i < files.size() && ret; ++i){
                    //Ignore .. and .
                    if ((files[i] != UP_DIR) && (files[i] != THIS_DIR)){
                        std::string originalPath = pathJoin(originalDir, files[i]);
                        std::string newPath = pathJoin(destinationPath, files[i]);
                        if (isFile(originalPath) == FILE_EQUAL){
                            ret = copyFile(originalPath, newPath);
                        }
                        else if (isDir(originalPath) == FILE_EQUAL){
                            //Create the new dir
                            ret = createDir(originalPath);
                            if (ret){
                                ret = copyDir(originalPath, newPath);
                            }
                        }
                        //Else ignore
                    }
                }
            }
            else{
                ret = false;
            }
        }
        else{
            ret = false;
        }
    }
    else{
        ret = false;
    }
    return ret;
}

bool FileSystem::renameFile(const std::string &originalFile, const std::string &newFileName){
    bool ret = false;
    if (originalFile == newFileName){
        ret = true; //No op
    }
    else{
        int status = m_cstdio->rename(originalFile, newFileName);
        ret = (status == 0);
    }
    return ret;
}

bool FileSystem::renameDir(const std::string &originalDir, const std::string &newDirName){
    return renameFile(originalDir, newDirName);
}

bool FileSystem::deleteFile(const std::string &filePath){
    int status = m_cstdio->remove(filePath);
    return status == 0;
}

bool FileSystem::deleteDir (const std::string &dirPath){
    std::deque<std::string> files;
    bool ret = listFilesInDir(dirPath, files);
    for (unsigned int i = 0; i < files.size() && ret; ++i){
        //Ignore .  and ..
        if ((files[i] != UP_DIR) && (files[i] != THIS_DIR)){
            std::string path = pathJoin(dirPath, files[i]);
            if (isFile(path) == FILE_EQUAL){
                ret = deleteFile(path);
            }
            else if (isDir(path) == FILE_EQUAL){
                ret = deleteDir(path);
            }
            //Else ignore
        }
    }
    //Finally, delete the root dir
    if (ret){
		#ifdef MSVC
			int status = RemoveDirectoryA(dirPath.c_str());
			ret = (status != 0);
		#else	
			int status = rmdir(dirPath.c_str());
			ret = (status == 0);
		#endif
    }
    return ret;
}

bool FileSystem::listFilesInDir(const std::string &dirPath, std::deque<std::string> &fileNamesInDir){

    #ifdef UNIT_TEST
		if (m_failListFilesInDir){
			return false;
		}
    #endif // UNIT_TEST

	bool ret = false;

	#ifdef MSVC
		HANDLE hFind;
		WIN32_FIND_DATAA data;

		std::string glob = dirPath + "/*.*";
		hFind = FindFirstFileA(glob.c_str(), &data);
		if (hFind == INVALID_HANDLE_VALUE) {
			ret = false;
		}
		else{
			do {
				fileNamesInDir.push_back(std::string(data.cFileName));
			} while (FindNextFileA(hFind, &data));
			FindClose(hFind);
			ret = true;
		}
	#else
		DIR *dir;
		dir = opendir(dirPath.c_str());
		if (dir == NULL){
			ret = false;
		}
		else{
			dirent *de = readdir(dir);
			while (de != NULL){
				fileNamesInDir.push_back(de->d_name);
				de = readdir(dir);
			}
			closedir(dir);
			ret = true;
		}
	#endif
    return ret;
}

FileSystem::FileStatus FileSystem::compareFiles(const std::string &fileOne, const std::string &fileTwo){

    FileSystem::FileStatus ret = FILE_EQUAL;

    //Ensure both are files
    if ((isFile(fileOne) != FILE_EQUAL) || (isFile(fileTwo) != FILE_EQUAL)){
        ret = FILE_ERROR;
    }
    else if (fileOne == fileTwo){
        ret =  FILE_EQUAL; //No op
    }
    else{

        std::stringstream file1SS;
        std::stringstream file2SS;

        std::ifstream inFile1;
        std::ifstream inFile2;

        inFile1.open(fileOne.c_str());
        if (inFile1.fail()){
            ret = FILE_ERROR;
        }
        else{
            inFile1.peek();
            while (!inFile1.eof() && !inFile1.fail()){
                file1SS.put(inFile1.get());
                inFile1.peek();
            }
            inFile1.close();
            if (inFile1.fail()){
                ret = FILE_ERROR;
            }
            else{
                inFile2.open(fileTwo.c_str());
                if (inFile2.fail()){
                    ret = FILE_ERROR;
                }
                else{
                    inFile2.peek();
                    while(!inFile2.eof() && !inFile2.fail()){
                        file2SS.put(inFile2.get());
                        inFile2.peek();
                    }
                    inFile2.close();
                    if (inFile2.fail()){
                        ret = FILE_ERROR;
                    }
                    else{
                        if (file1SS.str() == file2SS.str()){
                            ret = FILE_EQUAL;
                        }
                        else{
                            ret = FILE_NOT_EQUAL;
                        }
                    }
                }
            }
        }
    }

    return ret;
}

FileSystem::FileStatus FileSystem::compareDirs(const std::string &dirOne, const std::string &dirTwo){
    FileSystem::FileStatus status = FILE_EQUAL;
    std::deque <std::string> dir1Contents;
    std::deque <std::string> dir2Contents;

    //Ensure both are dirs
    if ((isDir(dirOne) != FILE_EQUAL) || (isDir(dirTwo) != FILE_EQUAL)){
        status = FILE_ERROR;
    }
    //If both are the same name, they must be the same dir
    else if (dirOne == dirTwo){
        status = FILE_EQUAL;
    }
    else if (listFilesInDir(dirOne, dir1Contents) && listFilesInDir(dirTwo, dir2Contents)){
        //Ensure the sizes are the same
        if (dir1Contents.size() == dir2Contents.size()){
            //Sort both
            std::sort (dir1Contents.begin(), dir1Contents.end());
            std::sort (dir2Contents.begin(), dir2Contents.end());
            //Ensure all the names match
            for (unsigned int i = 0; (i < dir1Contents.size()) && (status == FILE_EQUAL); ++i){
                std::string path1 = pathJoin(dirOne, dir1Contents[i]);
                std::string path2 = pathJoin(dirTwo, dir2Contents[i]);

                if (dir1Contents[i] != dir2Contents[i]){
                    status = FILE_NOT_EQUAL;
                }
                //If both are directories
                else if ((isDir(path1) == FILE_EQUAL) && (isDir(path2) == FILE_EQUAL)){
                    //Only compare dirs ifthey are not "." or ".."
                    if (dir1Contents[i] != UP_DIR && dir1Contents[i] != THIS_DIR){
                        status = compareDirs(pathJoin(dirOne, dir1Contents[i]), pathJoin(dirTwo, dir2Contents[i]));
                    }
                }
                //If both are files
                else if ((isFile(path1) == FILE_EQUAL) && (isFile(path2) == FILE_EQUAL)){
                    if (dir1Contents[i] != UP_DIR && dir1Contents[i] != THIS_DIR){
                        status = compareFiles(pathJoin(dirOne, dir1Contents[i]), pathJoin(dirTwo, dir2Contents[i]));
                    }
                }
                //The file types do not match
                else{
                    status = FILE_NOT_EQUAL;
                }
            }
        }
        else{
            status = FILE_NOT_EQUAL;
        }
    }
    else{
        status = FILE_ERROR;
    }
    return status;
}

}
