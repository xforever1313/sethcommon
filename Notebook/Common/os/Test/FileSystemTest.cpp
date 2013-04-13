#include <boost/test/unit_test.hpp>
#include <fstream>
#include <sstream>

#define private public

#include "FileSystem.h"
#include "TestHelper.h"

std::string illegalDir;

///\brief creates a massive directory, with dirs, files, and other things
void createMassiveDir(FileSystem uut, std::string rootPath){

    BOOST_CHECK(uut.createDir(rootPath));
    BOOST_CHECK(uut.dirExists(rootPath));

    BOOST_CHECK(uut.createDir(uut.pathJoin(rootPath, "dir1/dir1Child")));
    BOOST_CHECK(uut.dirExists(uut.pathJoin(rootPath, "dir1")));
    BOOST_CHECK(uut.dirExists(uut.pathJoin(rootPath, "dir1/dir1Child")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(rootPath, "dir1/test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(rootPath, "dir1/test1.txt")));
    std::ofstream outFile1;
    outFile1.open("dir1/dir1Child/test1.txt");
    outFile1<<"Test File!\n\nYAY!";
    outFile1.close();

    BOOST_CHECK(uut.createDir(uut.pathJoin(rootPath, "dir2/dir2Child/dir2ChildChild")));
    BOOST_CHECK(uut.dirExists(uut.pathJoin(rootPath, "dir2")));
    BOOST_CHECK(uut.dirExists(uut.pathJoin(rootPath, "dir2/dir2Child")));
    BOOST_CHECK(uut.dirExists(uut.pathJoin(rootPath, "dir2/dir2Child/dir2ChildChild")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(rootPath, "dir2/dir2Child/test.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(rootPath, "dir2/dir2Child/test.txt")));
    std::ofstream outFile2;
    outFile2.open("dir2/dir2Child/dir2ChildChild/test1.txt");
    outFile2<<"Test File2!\n\nYAY!";
    outFile2.close();

    //Create files
    BOOST_CHECK(uut.createFile(uut.pathJoin(rootPath, "testFile1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(rootPath, "testFile1.txt")));

}

///\brief setup
BOOST_AUTO_TEST_CASE(FilesSystem_setUp){
    //First, create the directory we are going to be in
    FileSystem uut;
    BOOST_CHECK(uut.createDir(fileTestOutputPath));
    BOOST_CHECK(uut.dirExists(fileTestOutputPath));
    #if defined(_WIN32) | defined(_WIN64)
        illegalDir = "con";
    #else
        illegalDir = "/lost+found/derp";
    #endif
}

///\brief tests the file creation method
BOOST_AUTO_TEST_CASE(FileSystem_createFileTest){
    std::string testFile = "createFileTest.txt";
    std::stringstream ss;
    ss << fileTestOutputPath << "/" << testFile;

    FileSystem uut;

    //Test the empty string case
    BOOST_CHECK(!uut.createFile(""));

    BOOST_CHECK(uut.createFile(ss.str()));
    BOOST_CHECK(uut.fileExists(ss.str()));
}

///\brief tests the create dir method
BOOST_AUTO_TEST_CASE(FileSystem_createDirTest){
    std::string testDir1 = "createdDir1";
    std::string testDir2 = "createdDir2";
    std::string testDirChild1 = "createdDirChild1";
    std::string testDirChild2 = "createdDirChild2";

    FileSystem uut;
    std::string testDirLocation1 = uut.pathJoin(fileTestOutputPath, testDir1);

    BOOST_CHECK(uut.createDir(testDirLocation1));
    BOOST_CHECK(uut.dirExists(testDirLocation1));

    //Test the empty string case
    BOOST_CHECK(!uut.createDir(""));

    //Test the case where we want to create a directory within a directory
    std::string testDirLocation2 = uut.pathJoin(fileTestOutputPath, testDir2);
    testDirLocation2 = uut.pathJoin(testDirLocation2, testDirChild1);
    BOOST_CHECK(uut.createDir(testDirLocation2));
    BOOST_CHECK(uut.dirExists(testDirLocation2));

    //Test the case where we want to create a directory within an existing diretory
    std::string testDirLocation3 = uut.pathJoin(fileTestOutputPath, testDir2);
    testDirLocation3 = uut.pathJoin(testDirLocation3, testDirChild2);
    BOOST_CHECK(uut.createDir(testDirLocation3));
    BOOST_CHECK(uut.dirExists(testDirLocation3));

	#ifdef _WIN32
    BOOST_CHECK(!uut.createDir(uut.pathJoin(fileTestOutputPath, illegalDir)));
    #else
    BOOST_CHECK(!uut.createDir(illegalDir));
    #endif
}

///\brief tests the isFile method
BOOST_AUTO_TEST_CASE(FileSystem_isFileTest){
    FileSystem uut;
    BOOST_CHECK_EQUAL(uut.isFile(fileTestOutputPath), FileSystem::FILE_NOT_EQUAL);
    BOOST_CHECK_EQUAL(uut.isFile(unEditableFilePath), FileSystem::FILE_EQUAL);
    BOOST_CHECK_EQUAL(uut.isFile("derp.txt"), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.isFile(""), FileSystem::FILE_ERROR);
}

///\brief tests the isDir method
BOOST_AUTO_TEST_CASE(FileSystem_isDirTest){
    FileSystem uut;
    BOOST_CHECK_EQUAL(uut.isDir(fileTestOutputPath), FileSystem::FILE_EQUAL);
    BOOST_CHECK_EQUAL(uut.isDir(unEditableFilePath), FileSystem::FILE_NOT_EQUAL);
    BOOST_CHECK_EQUAL(uut.isDir("derp"), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.isFile(""), FileSystem::FILE_ERROR);
}

///\brief tests the file exists method
BOOST_AUTO_TEST_CASE(FileSystem_fileExistsTest){
    FileSystem uut;
    BOOST_CHECK(uut.fileExists(unEditableFilePath.c_str()));
    //BOOST_CHECK(!uut.fileExists("derp.txt"));
}

///\brief tests the directory exists method
BOOST_AUTO_TEST_CASE(FileSystem_DirExistsTest){
    FileSystem uut;
    BOOST_CHECK(uut.dirExists(fileTestOutputPath));
    BOOST_CHECK(!uut.dirExists("derp"));
}

///\brief tests the copyFile method
BOOST_AUTO_TEST_CASE(FileSystem_copyFileTest){
    std::string originalFile = "originalCopy.txt";
    std::string newFile = "copiedFile.txt";
    std::stringstream originalSS;
    std::stringstream copySS;
    originalSS << fileTestOutputPath << "/" << originalFile;
    copySS << fileTestOutputPath << "/" << newFile;

    //Make arbitrary original file
    std::ofstream outFile(originalSS.str().c_str());
    outFile << "Hello, this is a file that needs to be copied\n\n\nPretty cool right?";
    outFile.close();

    FileSystem uut;
    //Test same file name
    BOOST_CHECK(uut.copyFile(originalSS.str(), originalSS.str()));
    BOOST_CHECK(uut.fileExists(originalSS.str()));
    BOOST_CHECK(!uut.fileExists(copySS.str()));

    BOOST_CHECK(uut.copyFile(originalSS.str(), copySS.str()));
    BOOST_CHECK(uut.fileExists(originalSS.str()));
    BOOST_CHECK(uut.fileExists(copySS.str()));

    //Test file doesn't exist case
    BOOST_CHECK(!uut.copyFile("derp.txt", copySS.str()));

    //Test two empty files
    std::string blankFile1 = uut.pathJoin(fileTestOutputPath, "blankFile1");
    std::string blankFile2 = uut.pathJoin(fileTestOutputPath, "blankFile2");

    BOOST_CHECK(uut.createFile(blankFile1));
    BOOST_CHECK(uut.fileExists(blankFile1));
    BOOST_CHECK(uut.createFile(blankFile2));
    BOOST_CHECK(uut.fileExists(blankFile2));

    BOOST_CHECK(uut.copyFile(blankFile1, blankFile2));
}

///\brief tests the copyDir method
BOOST_AUTO_TEST_CASE(FileSystem_copyDirTest){
    FileSystem uut;

    std::string testDir = uut.pathJoin(fileTestOutputPath, "copyDir1");
    std::string copiedDir = uut.pathJoin(fileTestOutputPath, "copyDir2");
    createMassiveDir(uut, testDir);
    BOOST_CHECK(uut.copyDir(testDir, copiedDir));
    BOOST_CHECK_EQUAL(uut.compareDirs(testDir, copiedDir), FileSystem::FILE_EQUAL);

    //Test the case where the dirs have the same name
    BOOST_CHECK(!uut.copyDir(testDir, testDir));
    BOOST_CHECK(uut.dirExists(testDir));

    //Test the case where a directory does not exist
    BOOST_CHECK(!uut.copyDir("derp", testDir));
    BOOST_CHECK(!uut.copyDir("derp", "herp"));

    //Tests the case where we are copying to an illegal directory
    #ifdef _WIN32
    BOOST_CHECK(!uut.copyDir(testDir, uut.pathJoin(fileTestOutputPath, illegalDir)));
	#else
	BOOST_CHECK(!uut.copyDir(testDir, illegalDir));
	#endif
	
    //Test the case where listFiles fail
    uut.m_failListFilesInDir = true;
    copiedDir = uut.pathJoin(fileTestOutputPath, "copyDir3");
    BOOST_CHECK(!uut.copyDir(testDir, copiedDir));
}

///\brief tests the renameFile method
BOOST_AUTO_TEST_CASE(FileSystem_renameFileTest){
    std::string orginalFile = "toBeRenamed.txt";
    std::stringstream orginalSS;
    orginalSS << fileTestOutputPath << "/" << orginalFile;

    std::string newFile = "renamedFile.txt";
    std::stringstream newSS;
    newSS << fileTestOutputPath << "/" << newFile;

    FileSystem uut;
    //Ensure the orginal file was created
    BOOST_CHECK(uut.createFile(orginalSS.str().c_str()));
    BOOST_CHECK(uut.fileExists(orginalSS.str().c_str()));
    BOOST_CHECK(!uut.fileExists(newSS.str().c_str()));

    //Ensure when a file does not exist is send in, it is not created
    BOOST_CHECK(!uut.renameFile("derp.txt", newSS.str().c_str()));
    BOOST_CHECK(!uut.fileExists(newSS.str().c_str()));

    //Ensure the no op situation
    BOOST_CHECK(uut.renameFile(orginalSS.str().c_str(), orginalSS.str().c_str()));
    BOOST_CHECK(uut.fileExists(orginalSS.str().c_str()));
    BOOST_CHECK(!uut.fileExists(newSS.str().c_str()));

    //Ensure rename is successful by the old file not existing, and the new one existing
    BOOST_CHECK(uut.renameFile(orginalSS.str().c_str(), newSS.str().c_str()));
    BOOST_CHECK(!uut.fileExists(orginalSS.str().c_str()));
    BOOST_CHECK(uut.fileExists(newSS.str().c_str()));
}

///\brief tests the renameDir method
BOOST_AUTO_TEST_CASE(FileSystem_renameDirTest){
    std::string orginalDir = "dirToBeRenamed";
    std::stringstream orginalSS;
    orginalSS << fileTestOutputPath << "/" << orginalDir;

    std::string newDir = "renamedDir";
    std::stringstream newSS;
    newSS << fileTestOutputPath << "/" << newDir;

    FileSystem uut;
    //Ensure the orginal file was created
    BOOST_CHECK(uut.createDir(orginalSS.str().c_str()));
    BOOST_CHECK(uut.dirExists(orginalSS.str().c_str()));
    BOOST_CHECK(!uut.dirExists(newSS.str().c_str()));

    //Ensure when a file does not exist is send in, it is not created
    BOOST_CHECK(!uut.renameDir("derp", newSS.str().c_str()));
    BOOST_CHECK(!uut.dirExists(newSS.str().c_str()));

    //Ensure the no op situation
    BOOST_CHECK(uut.renameDir(orginalSS.str().c_str(), orginalSS.str().c_str()));
    BOOST_CHECK(uut.dirExists(orginalSS.str().c_str()));
    BOOST_CHECK(!uut.dirExists(newSS.str().c_str()));

    //Ensure rename is successful by the old file not existing, and the new one existing
    BOOST_CHECK(uut.renameDir(orginalSS.str().c_str(), newSS.str().c_str()));
    BOOST_CHECK(!uut.dirExists(orginalSS.str().c_str()));
    BOOST_CHECK(uut.dirExists(newSS.str().c_str()));
}

///\brief tests the movefile ability of renameFile
BOOST_AUTO_TEST_CASE(FileSystem_moveFileTest){
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

    FileSystem uut;
    //Create the needed directories and files
    BOOST_CHECK(uut.createDir(originalDirSS.str()));
    BOOST_CHECK(uut.dirExists(originalDirSS.str()));
    BOOST_CHECK(uut.createFile(originalFileSS.str()));
    BOOST_CHECK(uut.fileExists(originalFileSS.str()));
    BOOST_CHECK(uut.createDir(newDirSS.str()));
    BOOST_CHECK(uut.dirExists(newDirSS.str()));

    //Test moving
    BOOST_CHECK(uut.renameFile(originalFileSS.str(), newFileSS.str()));

    //Ensure the file was moved
    BOOST_CHECK(!uut.fileExists(originalFileSS.str()));
    BOOST_CHECK(uut.fileExists(newFileSS.str()));
}

///\brief tests the move dir ability of renameDir
BOOST_AUTO_TEST_CASE(FileSystem_moveDirTest){
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

    FileSystem uut;
    //Create the needed directories and files
    BOOST_CHECK(uut.createDir(originalDirLocationSS.str()));
    BOOST_CHECK(uut.dirExists(originalDirLocationSS.str()));
    BOOST_CHECK(uut.createDir(originalDirSS.str()));
    BOOST_CHECK(uut.dirExists(originalDirSS.str()));
    BOOST_CHECK(uut.createDir(newDirLocationSS.str()));
    BOOST_CHECK(uut.dirExists(newDirLocationSS.str()));

    //Test moving
    BOOST_CHECK(uut.renameDir(originalDirSS.str(), newDirSS.str()));

    //Ensure the file was moved
    BOOST_CHECK(!uut.dirExists(originalDirSS.str()));
    BOOST_CHECK(uut.dirExists(newDirSS.str()));
}

///\brief tests the delete file method
BOOST_AUTO_TEST_CASE(FileSystem_removeFileTest){
    std::string testFile = "toBeDeleted.txt";
    std::stringstream ss;
    ss << fileTestOutputPath << "/" << testFile;
    FileSystem uut;
    BOOST_CHECK(uut.createFile(ss.str().c_str()));
    BOOST_CHECK(uut.fileExists(ss.str().c_str()));
    BOOST_CHECK(uut.deleteFile(ss.str().c_str()));
    BOOST_CHECK(!uut.fileExists(ss.str().c_str()));
    BOOST_CHECK(!uut.deleteFile("derp.txt"));
}

///\brief tests the delete dir method
BOOST_AUTO_TEST_CASE(FileSystem_removeDirTest){
    std::string testDir = "toBeDeletedDir";
    std::stringstream ss;
    ss << fileTestOutputPath << "/" << testDir;
    FileSystem uut;
    BOOST_CHECK(uut.createDir(ss.str().c_str()));
    BOOST_CHECK(uut.dirExists(ss.str().c_str()));
    BOOST_CHECK(uut.deleteDir(ss.str().c_str()));
    BOOST_CHECK(!uut.dirExists(ss.str().c_str()));

    BOOST_CHECK(!uut.deleteDir("derp"));

    //Check for a massive dir
    std::string massiveDir = uut.pathJoin(fileTestOutputPath,"massiveToBeDeleted");
    createMassiveDir(uut, massiveDir);
    BOOST_CHECK(uut.deleteDir(massiveDir));
    BOOST_CHECK(!uut.dirExists(massiveDir));
}

///brief tests the list files in dir method
BOOST_AUTO_TEST_CASE(FileSystem_listFilesInDirTest){
    std::string testFile1 = "testFile1.txt";
    std::string testFile2 = "testFile2.txt";
    std::string testFile3 = "testFile3.txt";
    std::string testDirFile = "testDir";
    std::string testDir = "listDirTest";

    FileSystem uut;
    std::string uutDir = uut.pathJoin(fileTestOutputPath, testDir);
    BOOST_CHECK(uut.createDir(uutDir));
    BOOST_CHECK(uut.createDir(uut.pathJoin(uutDir, testDirFile)));
    BOOST_CHECK(uut.createFile(uut.pathJoin(uutDir, testFile1)));
    BOOST_CHECK(uut.createFile(uut.pathJoin(uutDir, testFile2)));
    BOOST_CHECK(uut.createFile(uut.pathJoin(uutDir, testFile3)));

    std::deque <std::string> files;
    BOOST_CHECK(uut.listFilesInDir(uutDir, files));
    BOOST_CHECK_EQUAL(files.size(), 6);  //6 due to ., .., and the three files
}

///\brief tests the compare Files method
BOOST_AUTO_TEST_CASE(FileSystem_compareFilesTest){
    std::string testFile1 = "sameFile1.txt";
    std::string testFile2 = "sameFile2.txt";
    std::string testFile3 = "differentFile.txt";
    std::string testDir = "compareFileTest";

    FileSystem uut;
    std::string uutDir = uut.pathJoin(fileTestOutputPath, testDir);
    BOOST_CHECK(uut.createDir(uutDir));
    BOOST_CHECK(uut.createFile(uut.pathJoin(uutDir, testFile1)));
    BOOST_CHECK(uut.createFile(uut.pathJoin(uutDir, testFile2)));
    BOOST_CHECK(uut.createFile(uut.pathJoin(uutDir, testFile3)));

    std::string sameString = "Hello, this string is between two files\n\ncool huh?";
    std::string differentString = "Different String!";

    std::ofstream sameFile1;
    std::ofstream sameFile2;
    std::ofstream differentFile;
    sameFile1.open(uut.pathJoin(uutDir, testFile1).c_str());
    sameFile1 << sameString;
    sameFile1.close();

    sameFile2.open(uut.pathJoin(uutDir, testFile2).c_str());
    sameFile2 << sameString;
    sameFile2.close();

    differentFile.open(uut.pathJoin(uutDir, testFile3).c_str());
    differentFile << differentString;
    differentFile.close();

    BOOST_CHECK_EQUAL(uut.compareFiles(uut.pathJoin(uutDir, testFile1), uut.pathJoin(uutDir, testFile2)), FileSystem::FILE_EQUAL);

    BOOST_CHECK_EQUAL(uut.compareFiles(uut.pathJoin(uutDir, testFile1), uut.pathJoin(uutDir, testFile3)), FileSystem::FILE_NOT_EQUAL);

    BOOST_CHECK_EQUAL(uut.compareFiles(uut.pathJoin(uutDir, testFile1),uut.pathJoin(uutDir, testFile1)), FileSystem::FILE_EQUAL);

    //Test the bad param cases
    BOOST_CHECK_EQUAL(uut.compareFiles(testFileDirectory, uut.pathJoin(uutDir, testFile1)), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareFiles(uut.pathJoin(uutDir, testFile1), testFileDirectory), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareFiles(testFileDirectory, testFileDirectory), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareFiles("DERP.txt", uut.pathJoin(uutDir, testFile3)), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareFiles(uut.pathJoin(uutDir, testFile1), "HERP.txt"), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareFiles("DERP.txt", "HERP.txt"), FileSystem::FILE_ERROR);
}

///\brief tests the compare directories method
BOOST_AUTO_TEST_CASE(FileSystem_compareDirsTest){
    FileSystem uut;

    std::string testDir = uut.pathJoin(fileTestOutputPath,"compareDirTest");
    //Setup
    BOOST_CHECK(uut.createDir(testDir));
    BOOST_CHECK(uut.dirExists(testDir));

    //Test the case where both dirs have the same path
    BOOST_CHECK_EQUAL(uut.compareDirs(testDir, testDir), FileSystem::FILE_EQUAL);
    BOOST_CHECK(uut.dirExists(testDir));

    //Test the case where neither dirs exist
    BOOST_CHECK_EQUAL(uut.compareDirs("derp", "derp"), FileSystem::FILE_ERROR);
    //Test the case where both are files
    BOOST_CHECK_EQUAL(uut.compareDirs(unEditableFilePath, unEditableFilePath), FileSystem::FILE_ERROR);
    //Test the case where ones a dir and one is not
    BOOST_CHECK_EQUAL(uut.compareDirs(testDir, unEditableFilePath), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareDirs(unEditableFilePath, testDir), FileSystem::FILE_ERROR);
    //Test the care where ones a dir, and one does not exist
    BOOST_CHECK_EQUAL(uut.compareDirs(testDir, "derp"), FileSystem::FILE_ERROR);
    BOOST_CHECK_EQUAL(uut.compareDirs("derp", testDir), FileSystem::FILE_ERROR);

    //Set up two dirs with different number of files in each
    std::string differentSizeDir = uut.pathJoin(testDir, "differentSizeDirTest");
    BOOST_CHECK(uut.createDir(differentSizeDir));
    BOOST_CHECK(uut.dirExists(differentSizeDir));

    std::string differentSizeDir1 = uut.pathJoin(differentSizeDir, "differentSizeDir1");
    BOOST_CHECK(uut.createDir(differentSizeDir1));
    BOOST_CHECK(uut.dirExists(differentSizeDir1));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentSizeDir1, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentSizeDir1, "test1.txt")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentSizeDir1, "test2.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentSizeDir1, "test2.txt")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentSizeDir1, "test3.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentSizeDir1, "test3.txt")));

    std::string differentSizeDir2 = uut.pathJoin(differentSizeDir, "differentSizeDir2");
    BOOST_CHECK(uut.createDir(differentSizeDir2));
    BOOST_CHECK(uut.dirExists(differentSizeDir2));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentSizeDir2, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentSizeDir2, "test1.txt")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentSizeDir2, "test2.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentSizeDir2, "test2.txt")));

    BOOST_CHECK_EQUAL(uut.compareDirs(differentSizeDir1, differentSizeDir2), FileSystem::FILE_NOT_EQUAL);

    //Set up two directories with miss matching names
    std::string differentFileNameDir = uut.pathJoin(testDir, "differentFileNames");
    BOOST_CHECK(uut.createDir(differentFileNameDir));
    BOOST_CHECK(uut.dirExists(differentFileNameDir));

    std::string differentFileNameDir1 = uut.pathJoin(differentFileNameDir, "differentFileNameDir1");
    BOOST_CHECK(uut.createDir(differentFileNameDir1));
    BOOST_CHECK(uut.dirExists(differentFileNameDir1));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileNameDir1, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileNameDir1, "test1.txt")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileNameDir1, "LOL.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileNameDir1, "LOL.txt")));

    std::string differentFileNameDir2 = uut.pathJoin(differentFileNameDir, "differentFileNameDir2");
    BOOST_CHECK(uut.createDir(differentFileNameDir2));
    BOOST_CHECK(uut.dirExists(differentFileNameDir2));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileNameDir2, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileNameDir2, "test1.txt")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileNameDir2, "test2.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileNameDir2, "test2.txt")));

    BOOST_CHECK_EQUAL(uut.compareDirs(differentFileNameDir1, differentFileNameDir2), FileSystem::FILE_NOT_EQUAL);

    //Set up two directories with miss matching types, but same names
    std::string differentFileTypesDir = uut.pathJoin(testDir, "differentFileTypesDir");
    BOOST_CHECK(uut.createDir(differentFileTypesDir));
    BOOST_CHECK(uut.dirExists(differentFileTypesDir));

    std::string differentFileTypesDir1 = uut.pathJoin(differentFileTypesDir, "differentFileTypesDir1");
    BOOST_CHECK(uut.createDir(differentFileTypesDir1));
    BOOST_CHECK(uut.dirExists(differentFileTypesDir1));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileTypesDir1, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileTypesDir1, "test1.txt")));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileTypesDir1, "test2")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileTypesDir1, "test2")));

    std::string differentFileTypesDir2 = uut.pathJoin(differentFileTypesDir, "differentFileTypesDir2");
    BOOST_CHECK(uut.createDir(differentFileTypesDir2));
    BOOST_CHECK(uut.dirExists(differentFileTypesDir2));
    BOOST_CHECK(uut.createFile(uut.pathJoin(differentFileTypesDir2, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(differentFileTypesDir2, "test1.txt")));
    BOOST_CHECK(uut.createDir(uut.pathJoin(differentFileTypesDir2, "test2")));
    BOOST_CHECK(uut.dirExists(uut.pathJoin(differentFileTypesDir2, "test2")));

    BOOST_CHECK_EQUAL(uut.compareDirs(differentFileTypesDir1, differentFileTypesDir2), FileSystem::FILE_NOT_EQUAL);

    //Set up two directories with just files
    std::string sameDirWithJustFiles = uut.pathJoin(testDir, "sameDirWithJustFiles");
    BOOST_CHECK(uut.createDir(sameDirWithJustFiles));
    BOOST_CHECK(uut.dirExists(sameDirWithJustFiles));

    std::string sameDirWithJustFiles1 = uut.pathJoin(sameDirWithJustFiles, "sameDirWithJustFiles1");
    BOOST_CHECK(uut.createDir(sameDirWithJustFiles1));
    BOOST_CHECK(uut.dirExists(sameDirWithJustFiles1));
    BOOST_CHECK(uut.createFile(uut.pathJoin(sameDirWithJustFiles1, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(sameDirWithJustFiles1, "test1.txt")));
    std::ofstream outFile1;
    outFile1.open((uut.pathJoin(sameDirWithJustFiles1, "test2.txt").c_str()));
    outFile1 << sameDirWithJustFiles << "\n\n" << sameDirWithJustFiles;
    outFile1.close();

    std::string sameDirWithJustFiles2 = uut.pathJoin(sameDirWithJustFiles, "sameDirWithJustFiles2");
    BOOST_CHECK(uut.createDir(sameDirWithJustFiles2));
    BOOST_CHECK(uut.dirExists(sameDirWithJustFiles2));
    BOOST_CHECK(uut.createFile(uut.pathJoin(sameDirWithJustFiles2, "test1.txt")));
    BOOST_CHECK(uut.fileExists(uut.pathJoin(sameDirWithJustFiles2, "test1.txt")));
    std::ofstream outFile2;
    outFile2.open((uut.pathJoin(sameDirWithJustFiles2, "test2.txt").c_str()));
    outFile2 << sameDirWithJustFiles << "\n\n" << sameDirWithJustFiles;
    outFile2.close();

    BOOST_CHECK_EQUAL(uut.compareDirs(sameDirWithJustFiles1, sameDirWithJustFiles2), FileSystem::FILE_EQUAL);

    //Set up two directories files and dirs
    //Tested in copyDirTest

    //Test the case where directory listing fails
    uut.m_failListFilesInDir = true;
    BOOST_CHECK_EQUAL(uut.compareDirs(sameDirWithJustFiles1, sameDirWithJustFiles2), FileSystem::FILE_ERROR);
}

BOOST_AUTO_TEST_CASE(FileSystem_pathJoinTest){
    std::string parent = "parent";
    std::string child = "child";
    std::stringstream ss;
    ss << parent << "/" << child;

    FileSystem uut;
    BOOST_CHECK_EQUAL(uut.pathJoin(parent, child),ss.str());
}
