
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <gmock/gmock.h>
#include <CppUTest/TestHarness.h>
#include <fstream>
#include <sstream>

#define private public

#include "FileSystem.h"
#include "cstdioWrapper.h"
#include "MockcstdioWrapper.h"
#include "TestHelper.h"

std::string illegalDir;

TEST_GROUP(FileSystem){
    bool firstRun = true;

    TEST_SETUP(){
        m_file = "File";
        m_stringToWrite = "Writing!";
        m_cstdio = nullptr;
        m_uut = new OS::FileSystem();
        if (firstRun){
            firstSetup();
            firstRun = false;
        }
    }
    TEST_TEARDOWN(){
        delete m_uut;
    }
    ///\brief setup
    void firstSetup(){
        //First, create the directory we are going to be in
        CHECK(m_uut->createDir(fileTestOutputPath));
        CHECK(m_uut->dirExists(fileTestOutputPath));
        #ifdef WIN32
            illegalDir = "con";
        #else
            illegalDir = "/lost+found/derp";
        #endif
    }
    
    void addMock(){
        m_cstdio = new testing::StrictMock<OS::MockcstdioWrapper>();
        delete m_uut->m_cstdio;
        m_uut->m_cstdio = m_cstdio;
    }
    std::string m_file;
    std::string m_stringToWrite;
    testing::StrictMock<OS::MockcstdioWrapper> *m_cstdio;
    OS::FileSystem *m_uut;
};

///\brief creates a massive directory, with dirs, files, and other things
void createMassiveDir(OS::FileSystem *uut, std::string rootPath){

    CHECK(uut->createDir(rootPath));
    CHECK(uut->dirExists(rootPath));

    CHECK(uut->createDir(OS::FileSystem::pathJoin(rootPath, "dir1/dir1Child")));
    CHECK(uut->dirExists(OS::FileSystem::pathJoin(rootPath, "dir1")));
    CHECK(uut->dirExists(OS::FileSystem::pathJoin(rootPath, "dir1/dir1Child")));
    CHECK(uut->createFile(OS::FileSystem::pathJoin(rootPath, "dir1/test1.txt")));
    CHECK(uut->fileExists(OS::FileSystem::pathJoin(rootPath, "dir1/test1.txt")));
    std::ofstream outFile1;
    outFile1.open("dir1/dir1Child/test1.txt");
    outFile1<<"Test File!\n\nYAY!";
    outFile1.close();

    CHECK(uut->createDir(OS::FileSystem::pathJoin(rootPath, "dir2/dir2Child/dir2ChildChild")));
    CHECK(uut->dirExists(OS::FileSystem::pathJoin(rootPath, "dir2")));
    CHECK(uut->dirExists(OS::FileSystem::pathJoin(rootPath, "dir2/dir2Child")));
    CHECK(uut->dirExists(OS::FileSystem::pathJoin(rootPath, "dir2/dir2Child/dir2ChildChild")));
    CHECK(uut->createFile(OS::FileSystem::pathJoin(rootPath, "dir2/dir2Child/test.txt")));
    CHECK(uut->fileExists(OS::FileSystem::pathJoin(rootPath, "dir2/dir2Child/test.txt")));
    std::ofstream outFile2;
    outFile2.open("dir2/dir2Child/dir2ChildChild/test1.txt");
    outFile2<<"Test File2!\n\nYAY!";
    outFile2.close();

    //Create files
    CHECK(uut->createFile(OS::FileSystem::pathJoin(rootPath, "testFile1.txt")));
    CHECK(uut->fileExists(OS::FileSystem::pathJoin(rootPath, "testFile1.txt")));
}

TEST(FileSystem, getInstanceTest){
    CHECK(OS::FileSystem::getInstance() == OS::FileSystem::getInstance());
}

///\brief tests the read file method during an open error
TEST(FileSystem, readFileOpenFailure){
    addMock();
    OS::FILE_t *file = nullptr;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("r")))
        .WillOnce(testing::Return(file));
    std::string buffer;
    CHECK(!m_uut->readFile(buffer, m_file));
    CHECK(buffer.empty());
}

///\brief tests the read file method during an open error
TEST(FileSystem, readFileReadFailure){
    addMock();
    testing::InSequence dummy;
    std::string fileString("qwerty");
    OS::FILE_t *file = new OS::FILE_t;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("r")))
        .WillOnce(testing::Return(file));
    EXPECT_CALL(*m_cstdio, fgetc(file))
        .WillOnce(testing::Return(fileString[0]));
    EXPECT_CALL(*m_cstdio, fgetc(file))
        .WillOnce(testing::Return(OS::cstdioWrapper::END_OF_FILE));
    EXPECT_CALL(*m_cstdio, ferror(file))
        .WillOnce(testing::Return(13));
    EXPECT_CALL(*m_cstdio, fclose(file))
        .WillOnce(testing::DoAll(testing::DeleteArg<0>(), testing::Return(0)));

    std::string buffer;
    CHECK(!m_uut->readFile(buffer, m_file));
    CHECK_EQUAL(buffer, "q");
}

///\brief tests the read file method during a success
TEST(FileSystem, readFileSuccess){
    addMock();
    testing::InSequence dummy;
    std::string expectedString = "Hello world!\n\nThis is a string!\t  cool huh?";
    OS::FILE_t *file = new OS::FILE_t;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("r"))).
        WillOnce(testing::Return(file));
    for (size_t i = 0; i < expectedString.size(); ++i){
        EXPECT_CALL(*m_cstdio, fgetc(file))
            .WillOnce(testing::Return(expectedString[i]));
    }
    EXPECT_CALL(*m_cstdio, fgetc(file))
        .WillOnce(testing::Return(OS::cstdioWrapper::END_OF_FILE));
    EXPECT_CALL(*m_cstdio, ferror(file))
        .WillOnce(testing::Return(0));
    EXPECT_CALL(*m_cstdio, fclose(file))
        .WillOnce(testing::DoAll(testing::DeleteArg<0>(), testing::Return(0)));

    std::string buffer;
    CHECK(m_uut->readFile(buffer, m_file));
    CHECK_EQUAL(buffer, expectedString);
}

///\brief tests the write file method during an open error
TEST(FileSystem, writeFileOpenFailure){
    addMock();
    OS::FILE_t *file = nullptr;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("w")))
        .WillOnce(testing::Return(file));
    CHECK(!m_uut->writeFile(m_stringToWrite, m_file));
}

///\brief tests the write file during a write error
TEST(FileSystem, writeFileWriteFailure){
    addMock();
    testing::InSequence dummy;
    OS::FILE_t *file = new OS::FILE_t;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("w")))
        .WillOnce(testing::Return(file));
    EXPECT_CALL(*m_cstdio, fputc(m_stringToWrite[0], file))
        .WillOnce(testing::Return(m_stringToWrite[0]));
    EXPECT_CALL(*m_cstdio, fputc(m_stringToWrite[1], file))
        .WillOnce(testing::Return(OS::cstdioWrapper::END_OF_FILE));
    EXPECT_CALL(*m_cstdio, ferror(file))
        .WillOnce(testing::Return(234));
    EXPECT_CALL(*m_cstdio, fclose(file))
        .WillOnce(testing::DoAll(testing::DeleteArg<0>(), testing::Return(0)));

    CHECK(!m_uut->writeFile(m_stringToWrite, m_file));
}

///\brief tests the write file during a write success
TEST(FileSystem, writeFileWriteSuccess){
    addMock();
    testing::InSequence dummy;
    OS::FILE_t *file = new OS::FILE_t;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("w")))
        .WillOnce(testing::Return(file));
    for (size_t i = 0; i < m_stringToWrite.size(); ++i){
        EXPECT_CALL(*m_cstdio, fputc(m_stringToWrite[i], file))
            .WillOnce(testing::Return(m_stringToWrite[i]));
    }
    EXPECT_CALL(*m_cstdio, ferror(file))
        .WillOnce(testing::Return(0));
    EXPECT_CALL(*m_cstdio, fclose(file))
        .WillOnce(testing::DoAll(testing::DeleteArg<0>(), testing::Return(0)));

    CHECK(m_uut->writeFile(m_stringToWrite, m_file));
}

///\brief tests the file creation method for failure
TEST(FileSystem, createFileTestFaiure){
    addMock();
    OS::FILE_t *fileptr = nullptr;

    //Check the case where a file is bad
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("w")))
        .WillOnce(testing::Return(fileptr));
    CHECK(!m_uut->createFile(m_file));
}

///\brief tests the file creation method for success
TEST(FileSystem, createFileTestSuccess){
    addMock();
    OS::FILE_t *fileptr = new OS::FILE_t;
    EXPECT_CALL(*m_cstdio, fopen(m_file, std::string("w")))
        .WillOnce(testing::Return(fileptr));
    EXPECT_CALL(*m_cstdio, fclose(fileptr))
        .WillOnce(testing::DoAll(testing::DeleteArg<0>(), testing::Return(0)));

    CHECK(m_uut->createFile(m_file));
}

///\brief tests the create dir method
TEST(FileSystem, createDirTest){
    std::string testDir1 = "createdDir1";
    std::string testDir2 = "createdDir2";
    std::string testDirChild1 = "createdDirChild1";
    std::string testDirChild2 = "createdDirChild2";

    std::string testDirLocation1 = OS::FileSystem::pathJoin(fileTestOutputPath, testDir1);

    CHECK(m_uut->createDir(testDirLocation1));
    CHECK(m_uut->dirExists(testDirLocation1));

    //Test the empty string case
    CHECK(!m_uut->createDir(""));

    //Test the case where we want to create a directory within a directory
    std::string testDirLocation2 = OS::FileSystem::pathJoin(fileTestOutputPath, testDir2);
    testDirLocation2 = OS::FileSystem::pathJoin(testDirLocation2, testDirChild1);
    CHECK(m_uut->createDir(testDirLocation2));
    CHECK(m_uut->dirExists(testDirLocation2));

    //Test the case where we want to create a directory within an existing diretory
    std::string testDirLocation3 = OS::FileSystem::pathJoin(fileTestOutputPath, testDir2);
    testDirLocation3 = OS::FileSystem::pathJoin(testDirLocation3, testDirChild2);
    CHECK(m_uut->createDir(testDirLocation3));
    CHECK(m_uut->dirExists(testDirLocation3));

	#ifdef WIN32
    CHECK(!m_uut->createDir(OS::FileSystem::pathJoin(fileTestOutputPath, illegalDir)));
    #else
    CHECK(!m_uut->createDir(illegalDir));
    #endif
}

///\brief tests the case where the directory is an abspath from root
TEST(FileSystem, createDirFromRootTest){
    std::string testDir = "absDir";
    std::string absPath = m_uut->getCWD();
    std::string testDirLocation = OS::FileSystem::pathJoin(absPath, fileTestOutputPath);
    testDirLocation = OS::FileSystem::pathJoin(testDirLocation, testDir);
    CHECK(m_uut->createDir(testDirLocation));

    std::string relTestDirLocation = OS::FileSystem::pathJoin(fileTestOutputPath, testDir);
    CHECK(m_uut->dirExists(relTestDirLocation));
}

#ifndef MSVC //This test will pass on MSVC.  This is mainly to test linux systems.
///\brief tests the case where a directory tries to be made in root
TEST(FileSystem, createDirInRootTest){
    //You should not be able to create a dir in root.
    std::string testDir = "/sethderp";
    CHECK(!m_uut->createDir(testDir));
    CHECK(!m_uut->dirExists(testDir));
}
#endif

///\brief tests the case where the directory has two '//' in it
TEST(FileSystem, createDirDoubleSlashes){
    std::string testDir = "//twoSlashDir";
    std::string testDirLocation = OS::FileSystem::pathJoin(fileTestOutputPath, testDir);
    testDirLocation = OS::FileSystem::pathJoin(testDirLocation, testDir);
    CHECK(m_uut->createDir(testDirLocation));

    std::string realTestDirLocation = OS::FileSystem::pathJoin(fileTestOutputPath, "twoSlashDir");
    CHECK(m_uut->dirExists(realTestDirLocation));
}

///\brief tests the isFile method
TEST(FileSystem, isFileTest){
    CHECK_EQUAL(m_uut->isFile(fileTestOutputPath), OS::FileSystem::FILE_NOT_EQUAL);
    CHECK_EQUAL(m_uut->isFile(unEditableFilePath), OS::FileSystem::FILE_EQUAL);
    CHECK_EQUAL(m_uut->isFile("derp.txt"), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->isFile(""), OS::FileSystem::FILE_ERROR);
}

///\brief tests the isDir method
TEST(FileSystem, isDirTest){
    CHECK_EQUAL(m_uut->isDir(fileTestOutputPath), OS::FileSystem::FILE_EQUAL);
    CHECK_EQUAL(m_uut->isDir(unEditableFilePath), OS::FileSystem::FILE_NOT_EQUAL);
    CHECK_EQUAL(m_uut->isDir("derp"), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->isFile(""), OS::FileSystem::FILE_ERROR);
}

///\brief tests the file exists method
TEST(FileSystem, fileExistsTest){
    CHECK(m_uut->fileExists(unEditableFilePath));
    CHECK(!m_uut->fileExists("derp.txt"));
}

///\brief tests the directory exists method
TEST(FileSystem, DirExistsTest){
    CHECK(m_uut->dirExists(fileTestOutputPath));
    CHECK(!m_uut->dirExists("derp"));
}

///\brief tests the copyFile method
TEST(FileSystem, copyFileTest){
    std::string originalFile = "originalCopy.txt";
    std::string newFile = "copiedFile.txt";
    std::stringstream originalSS;
    std::stringstream copySS;
    originalSS << fileTestOutputPath << "/" << originalFile;
    copySS << fileTestOutputPath << "/" << newFile;

    //Make arbitrary original file
    std::ofstream outFile(originalSS.str());
    outFile << "Hello, this is a file that needs to be copied\n\n\nPretty cool right?";
    outFile.close();

    //Test same file name
    CHECK(m_uut->copyFile(originalSS.str(), originalSS.str()));
    CHECK(m_uut->fileExists(originalSS.str()));
    CHECK(!m_uut->fileExists(copySS.str()));

    CHECK(m_uut->copyFile(originalSS.str(), copySS.str()));
    CHECK(m_uut->fileExists(originalSS.str()));
    CHECK(m_uut->fileExists(copySS.str()));

    //Test file doesn't exist case
    CHECK(!m_uut->copyFile("derp.txt", copySS.str()));

    //Test two empty files
    std::string blankFile1 = OS::FileSystem::pathJoin(fileTestOutputPath, "blankFile1");
    std::string blankFile2 = OS::FileSystem::pathJoin(fileTestOutputPath, "blankFile2");

    CHECK(m_uut->createFile(blankFile1));
    CHECK(m_uut->fileExists(blankFile1));
    CHECK(m_uut->createFile(blankFile2));
    CHECK(m_uut->fileExists(blankFile2));

    CHECK(m_uut->copyFile(blankFile1, blankFile2));
}

///\brief tests the copyDir method
TEST(FileSystem, copyDirTest){
    std::string testDir = OS::FileSystem::pathJoin(fileTestOutputPath, "copyDir1");
    std::string copiedDir = OS::FileSystem::pathJoin(fileTestOutputPath, "copyDir2");
    createMassiveDir(m_uut, testDir);
    CHECK(m_uut->copyDir(testDir, copiedDir));
    CHECK_EQUAL(m_uut->compareDirs(testDir, copiedDir), OS::FileSystem::FILE_EQUAL);

    //Test the case where the dirs have the same name
    CHECK(!m_uut->copyDir(testDir, testDir));
    CHECK(m_uut->dirExists(testDir));

    //Test the case where a directory does not exist
    CHECK(!m_uut->copyDir("derp", testDir));
    CHECK(!m_uut->copyDir("derp", "herp"));

    //Tests the case where we are copying to an illegal directory
    #ifdef WIN32
    CHECK(!m_uut->copyDir(testDir, OS::FileSystem::pathJoin(fileTestOutputPath, illegalDir)));
	#else
	CHECK(!m_uut->copyDir(testDir, illegalDir));
	#endif

    //Test the case where listFiles fail
    m_uut->m_failListFilesInDir = true;
    copiedDir = OS::FileSystem::pathJoin(fileTestOutputPath, "copyDir3");
    CHECK(!m_uut->copyDir(testDir, copiedDir));
}

///\brief tests the renameFile method
TEST(FileSystem, renameFileTest){
    std::string orginalFile = "toBeRenamed.txt";
    std::stringstream orginalSS;
    orginalSS << fileTestOutputPath << "/" << orginalFile;

    std::string newFile = "renamedFile.txt";
    std::stringstream newSS;
    newSS << fileTestOutputPath << "/" << newFile;

    //Ensure the orginal file was created
    CHECK(m_uut->createFile(orginalSS.str()));
    CHECK(m_uut->fileExists(orginalSS.str()));
    CHECK(!m_uut->fileExists(newSS.str()));

    //Ensure when a file does not exist is send in, it is not created
    CHECK(!m_uut->renameFile("derp.txt", newSS.str()));
    CHECK(!m_uut->fileExists(newSS.str()));

    //Ensure the no op situation
    CHECK(m_uut->renameFile(orginalSS.str(), orginalSS.str()));
    CHECK(m_uut->fileExists(orginalSS.str()));
    CHECK(!m_uut->fileExists(newSS.str()));

    //Ensure rename is successful by the old file not existing, and the new one existing
    CHECK(m_uut->renameFile(orginalSS.str(), newSS.str()));
    CHECK(!m_uut->fileExists(orginalSS.str()));
    CHECK(m_uut->fileExists(newSS.str()));
}

///\brief tests the renameDir method
TEST(FileSystem, renameDirTest){
    std::string orginalDir = "dirToBeRenamed";
    std::string orginalPath;
    orginalPath = fileTestOutputPath + "/" + orginalDir;

    std::string newDir = "renamedDir";
    std::string newPath;
    newPath = fileTestOutputPath + "/" + newDir;

    //Ensure the orginal file was created
    CHECK(m_uut->createDir(orginalPath));
    CHECK(m_uut->dirExists(orginalPath));
    CHECK(!m_uut->dirExists(newPath));

    //Ensure when a file does not exist is send in, it is not created
    CHECK(!m_uut->renameDir("derp", newPath));
    CHECK(!m_uut->dirExists(newPath));

    //Ensure the no op situation
    CHECK(m_uut->renameDir(orginalPath, orginalPath));
    CHECK(m_uut->dirExists(orginalPath));
    CHECK(!m_uut->dirExists(newPath));

    //Ensure rename is successful by the old file not existing, and the new one existing
    CHECK(m_uut->renameDir(orginalPath, newPath));
    CHECK(!m_uut->dirExists(orginalPath));
    CHECK(m_uut->dirExists(newPath));
}

///\brief tests the movefile ability of renameFile
TEST(FileSystem, moveFileTest){
    std::string originalFile = "toBeMoved.txt";
    std::string originalDir = "moveFileTest1";
    std::string newFile = "movedFile.txt";
    std::string newDir = "moveFileTest2";

    std::stringstream originalDirSS;
    originalDirSS << fileTestOutputPath << "/" << originalDir;

    std::stringstream originalFileSS;
    originalFileSS << originalDirSS.str() << "/" << originalFile;

    std::stringstream newDirSS;
    newDirSS << fileTestOutputPath << "/" << newDir;

    std::stringstream newFileSS;
    newFileSS << newDirSS.str() << "/" << newFile;

    //Create the needed directories and files
    CHECK(m_uut->createDir(originalDirSS.str()));
    CHECK(m_uut->dirExists(originalDirSS.str()));
    CHECK(m_uut->createFile(originalFileSS.str()));
    CHECK(m_uut->fileExists(originalFileSS.str()));
    CHECK(m_uut->createDir(newDirSS.str()));
    CHECK(m_uut->dirExists(newDirSS.str()));

    //Test moving
    CHECK(m_uut->renameFile(originalFileSS.str(), newFileSS.str()));

    //Ensure the file was moved
    CHECK(!m_uut->fileExists(originalFileSS.str()));
    CHECK(m_uut->fileExists(newFileSS.str()));
}

///\brief tests the move dir ability of renameDir
TEST(FileSystem, moveDirTest){
    std::string originalDir = "toBeMoved";
    std::string originalDirLocation = "moveFileTest1";
    std::string newDir = "movedDir";
    std::string newDirLocation = "moveFileTest2";

    std::stringstream originalDirLocationSS;
    originalDirLocationSS << fileTestOutputPath << "/" << originalDirLocation;

    std::stringstream originalDirSS;
    originalDirSS << originalDirLocationSS.str() << "/" << originalDir;

    std::stringstream newDirLocationSS;
    newDirLocationSS << fileTestOutputPath << "/" << newDirLocation;

    std::stringstream newDirSS;
    newDirSS << newDirLocationSS.str() << "/" << newDir;

    //Create the needed directories and files
    CHECK(m_uut->createDir(originalDirLocationSS.str()));
    CHECK(m_uut->dirExists(originalDirLocationSS.str()));
    CHECK(m_uut->createDir(originalDirSS.str()));
    CHECK(m_uut->dirExists(originalDirSS.str()));
    CHECK(m_uut->createDir(newDirLocationSS.str()));
    CHECK(m_uut->dirExists(newDirLocationSS.str()));

    //Test moving
    CHECK(m_uut->renameDir(originalDirSS.str(), newDirSS.str()));

    //Ensure the file was moved
    CHECK(!m_uut->dirExists(originalDirSS.str()));
    CHECK(m_uut->dirExists(newDirSS.str()));
}

///\brief tests the delete file method
TEST(FileSystem, removeFileTestFailure){
    addMock();
    EXPECT_CALL(*m_cstdio, remove(m_file))
        .WillOnce(testing::Return(1));
    CHECK(!m_uut->deleteFile(m_file));
}

TEST(FileSystem, removeFileTestSuccess){
    addMock();
    EXPECT_CALL(*m_cstdio, remove(m_file))
        .WillOnce(testing::Return(0));
    CHECK(m_uut->deleteFile(m_file));
}

///\brief tests the delete dir method
TEST(FileSystem, removeDirTest){
    std::string testDir = "toBeDeletedDir";
    std::string dir;
    dir = fileTestOutputPath + "/" + testDir;
    CHECK(m_uut->createDir(dir));
    CHECK(m_uut->dirExists(dir));
    CHECK(m_uut->deleteDir(dir));
    CHECK(!m_uut->dirExists(dir));

    CHECK(!m_uut->deleteDir("derp"));

    //Check for a massive dir
    std::string massiveDir = OS::FileSystem::pathJoin(fileTestOutputPath,"massiveToBeDeleted");
    createMassiveDir(m_uut, massiveDir);
    CHECK(m_uut->deleteDir(massiveDir));
    CHECK(!m_uut->dirExists(massiveDir));
}

///brief tests the list files in dir method
TEST(FileSystem, listFilesInDirTest){
    std::string testFile1 = "testFile1.txt";
    std::string testFile2 = "testFile2.txt";
    std::string testFile3 = "testFile3.txt";
    std::string testDirFile = "testDir";
    std::string testDir = "listDirTest";

    std::string uutDir = OS::FileSystem::pathJoin(fileTestOutputPath, testDir);
    CHECK(m_uut->createDir(uutDir));
    CHECK(m_uut->createDir(OS::FileSystem::pathJoin(uutDir, testDirFile)));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(uutDir, testFile1)));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(uutDir, testFile2)));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(uutDir, testFile3)));

    std::deque <std::string> files;
    CHECK(m_uut->listFilesInDir(uutDir, files));
    CHECK_EQUAL(files.size(), 6);  //6 due to ., .., and the three files
}

///\brief tests the compare Files method
TEST(FileSystem, compareFilesTest){
    std::string testFile1 = "sameFile1.txt";
    std::string testFile2 = "sameFile2.txt";
    std::string testFile3 = "differentFile.txt";
    std::string testDir = "compareFileTest";

    std::string uutDir = OS::FileSystem::pathJoin(fileTestOutputPath, testDir);
    CHECK(m_uut->createDir(uutDir));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(uutDir, testFile1)));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(uutDir, testFile2)));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(uutDir, testFile3)));

    std::string sameString = "Hello, this string is between two files\n\ncool huh?";
    std::string differentString = "Different String!";

    std::ofstream sameFile1;
    std::ofstream sameFile2;
    std::ofstream differentFile;
    sameFile1.open(OS::FileSystem::pathJoin(uutDir, testFile1));
    sameFile1 << sameString;
    sameFile1.close();

    sameFile2.open(OS::FileSystem::pathJoin(uutDir, testFile2));
    sameFile2 << sameString;
    sameFile2.close();

    differentFile.open(OS::FileSystem::pathJoin(uutDir, testFile3));
    differentFile << differentString;
    differentFile.close();

    CHECK_EQUAL(m_uut->compareFiles(OS::FileSystem::pathJoin(uutDir, testFile1), OS::FileSystem::pathJoin(uutDir, testFile2)), OS::FileSystem::FILE_EQUAL);

    CHECK_EQUAL(m_uut->compareFiles(OS::FileSystem::pathJoin(uutDir, testFile1), OS::FileSystem::pathJoin(uutDir, testFile3)), OS::FileSystem::FILE_NOT_EQUAL);

    CHECK_EQUAL(m_uut->compareFiles(OS::FileSystem::pathJoin(uutDir, testFile1),OS::FileSystem::pathJoin(uutDir, testFile1)), OS::FileSystem::FILE_EQUAL);

    //Test the bad param cases
    CHECK_EQUAL(m_uut->compareFiles(testFileDirectory, OS::FileSystem::pathJoin(uutDir, testFile1)), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareFiles(OS::FileSystem::pathJoin(uutDir, testFile1), testFileDirectory), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareFiles(testFileDirectory, testFileDirectory), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareFiles("DERP.txt", OS::FileSystem::pathJoin(uutDir, testFile3)), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareFiles(OS::FileSystem::pathJoin(uutDir, testFile1), "HERP.txt"), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareFiles("DERP.txt", "HERP.txt"), OS::FileSystem::FILE_ERROR);
}

///\brief tests the compare directories method
TEST(FileSystem, compareDirsTest){
    std::string testDir = OS::FileSystem::pathJoin(fileTestOutputPath,"compareDirTest");
    //Setup
    CHECK(m_uut->createDir(testDir));
    CHECK(m_uut->dirExists(testDir));

    //Test the case where both dirs have the same path
    CHECK_EQUAL(m_uut->compareDirs(testDir, testDir), OS::FileSystem::FILE_EQUAL);
    CHECK(m_uut->dirExists(testDir));

    //Test the case where neither dirs exist
    CHECK_EQUAL(m_uut->compareDirs("derp", "derp"), OS::FileSystem::FILE_ERROR);
    //Test the case where both are files
    CHECK_EQUAL(m_uut->compareDirs(unEditableFilePath, unEditableFilePath), OS::FileSystem::FILE_ERROR);
    //Test the case where ones a dir and one is not
    CHECK_EQUAL(m_uut->compareDirs(testDir, unEditableFilePath), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareDirs(unEditableFilePath, testDir), OS::FileSystem::FILE_ERROR);
    //Test the care where ones a dir, and one does not exist
    CHECK_EQUAL(m_uut->compareDirs(testDir, "derp"), OS::FileSystem::FILE_ERROR);
    CHECK_EQUAL(m_uut->compareDirs("derp", testDir), OS::FileSystem::FILE_ERROR);

    //Set up two dirs with different number of files in each
    std::string differentSizeDir = OS::FileSystem::pathJoin(testDir, "differentSizeDirTest");
    CHECK(m_uut->createDir(differentSizeDir));
    CHECK(m_uut->dirExists(differentSizeDir));

    std::string differentSizeDir1 = OS::FileSystem::pathJoin(differentSizeDir, "differentSizeDir1");
    CHECK(m_uut->createDir(differentSizeDir1));
    CHECK(m_uut->dirExists(differentSizeDir1));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentSizeDir1, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentSizeDir1, "test1.txt")));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentSizeDir1, "test2.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentSizeDir1, "test2.txt")));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentSizeDir1, "test3.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentSizeDir1, "test3.txt")));

    std::string differentSizeDir2 = OS::FileSystem::pathJoin(differentSizeDir, "differentSizeDir2");
    CHECK(m_uut->createDir(differentSizeDir2));
    CHECK(m_uut->dirExists(differentSizeDir2));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentSizeDir2, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentSizeDir2, "test1.txt")));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentSizeDir2, "test2.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentSizeDir2, "test2.txt")));

    CHECK_EQUAL(m_uut->compareDirs(differentSizeDir1, differentSizeDir2), OS::FileSystem::FILE_NOT_EQUAL);

    //Set up two directories with miss matching names
    std::string differentFileNameDir = OS::FileSystem::pathJoin(testDir, "differentFileNames");
    CHECK(m_uut->createDir(differentFileNameDir));
    CHECK(m_uut->dirExists(differentFileNameDir));

    std::string differentFileNameDir1 = OS::FileSystem::pathJoin(differentFileNameDir, "differentFileNameDir1");
    CHECK(m_uut->createDir(differentFileNameDir1));
    CHECK(m_uut->dirExists(differentFileNameDir1));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileNameDir1, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileNameDir1, "test1.txt")));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileNameDir1, "LOL.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileNameDir1, "LOL.txt")));

    std::string differentFileNameDir2 = OS::FileSystem::pathJoin(differentFileNameDir, "differentFileNameDir2");
    CHECK(m_uut->createDir(differentFileNameDir2));
    CHECK(m_uut->dirExists(differentFileNameDir2));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileNameDir2, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileNameDir2, "test1.txt")));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileNameDir2, "test2.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileNameDir2, "test2.txt")));

    CHECK_EQUAL(m_uut->compareDirs(differentFileNameDir1, differentFileNameDir2), OS::FileSystem::FILE_NOT_EQUAL);

    //Set up two directories with miss matching types, but same names
    std::string differentFileTypesDir = OS::FileSystem::pathJoin(testDir, "differentFileTypesDir");
    CHECK(m_uut->createDir(differentFileTypesDir));
    CHECK(m_uut->dirExists(differentFileTypesDir));

    std::string differentFileTypesDir1 = OS::FileSystem::pathJoin(differentFileTypesDir, "differentFileTypesDir1");
    CHECK(m_uut->createDir(differentFileTypesDir1));
    CHECK(m_uut->dirExists(differentFileTypesDir1));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileTypesDir1, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileTypesDir1, "test1.txt")));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileTypesDir1, "test2")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileTypesDir1, "test2")));

    std::string differentFileTypesDir2 = OS::FileSystem::pathJoin(differentFileTypesDir, "differentFileTypesDir2");
    CHECK(m_uut->createDir(differentFileTypesDir2));
    CHECK(m_uut->dirExists(differentFileTypesDir2));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(differentFileTypesDir2, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(differentFileTypesDir2, "test1.txt")));
    CHECK(m_uut->createDir(OS::FileSystem::pathJoin(differentFileTypesDir2, "test2")));
    CHECK(m_uut->dirExists(OS::FileSystem::pathJoin(differentFileTypesDir2, "test2")));

    CHECK_EQUAL(m_uut->compareDirs(differentFileTypesDir1, differentFileTypesDir2), OS::FileSystem::FILE_NOT_EQUAL);

    //Set up two directories with just files
    std::string sameDirWithJustFiles = OS::FileSystem::pathJoin(testDir, "sameDirWithJustFiles");
    CHECK(m_uut->createDir(sameDirWithJustFiles));
    CHECK(m_uut->dirExists(sameDirWithJustFiles));

    std::string sameDirWithJustFiles1 = OS::FileSystem::pathJoin(sameDirWithJustFiles, "sameDirWithJustFiles1");
    CHECK(m_uut->createDir(sameDirWithJustFiles1));
    CHECK(m_uut->dirExists(sameDirWithJustFiles1));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(sameDirWithJustFiles1, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(sameDirWithJustFiles1, "test1.txt")));
    std::ofstream outFile1;
    outFile1.open((OS::FileSystem::pathJoin(sameDirWithJustFiles1, "test2.txt")));
    outFile1 << sameDirWithJustFiles << "\n\n" << sameDirWithJustFiles;
    outFile1.close();

    std::string sameDirWithJustFiles2 = OS::FileSystem::pathJoin(sameDirWithJustFiles, "sameDirWithJustFiles2");
    CHECK(m_uut->createDir(sameDirWithJustFiles2));
    CHECK(m_uut->dirExists(sameDirWithJustFiles2));
    CHECK(m_uut->createFile(OS::FileSystem::pathJoin(sameDirWithJustFiles2, "test1.txt")));
    CHECK(m_uut->fileExists(OS::FileSystem::pathJoin(sameDirWithJustFiles2, "test1.txt")));
    std::ofstream outFile2;
    outFile2.open((OS::FileSystem::pathJoin(sameDirWithJustFiles2, "test2.txt")));
    outFile2 << sameDirWithJustFiles << "\n\n" << sameDirWithJustFiles;
    outFile2.close();

    CHECK_EQUAL(m_uut->compareDirs(sameDirWithJustFiles1, sameDirWithJustFiles2), OS::FileSystem::FILE_EQUAL);

    //Set up two directories files and dirs
    //Tested in copyDirTest

    //Test the case where directory listing fails
    m_uut->m_failListFilesInDir = true;
    CHECK_EQUAL(m_uut->compareDirs(sameDirWithJustFiles1, sameDirWithJustFiles2), OS::FileSystem::FILE_ERROR);
}

TEST(FileSystem, pathJoinTest){
    std::string parent = "parent";
    std::string child = "child";
    std::stringstream ss;
    ss << parent << "/" << child;

    CHECK_EQUAL(OS::FileSystem::pathJoin(parent, child),ss.str());
}

#endif
