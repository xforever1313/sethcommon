
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASM_JS

#include <CppUTest/TestHarness.h>
#include <fstream>
#include <string>

#include "cstdioWrapper.h"
#include "Date.h"
#include "FileSystem.h"

TEST_GROUP(cstdioWrapper){
    bool firstRun = true;

    TEST_SETUP(){
        m_uut = (new OS::cstdioWrapper());
        m_fs = (OS::FileSystem::getInstance());
        m_testFile = ( "../../../Test/testFile.txt");
        m_testFileContents = ("test!");
        //Date constructed automatically
        m_testOutput = "../../../testOutput/cstdioWrapperTest/cstdioWrapperTest/" + m_date.getFullDateUnderscores();
        if (firstRun){
            CHECK(m_fs->createDir(m_testOutput));
            firstRun = false;
        }
    }

    TEST_TEARDOWN(){
        delete m_uut;
    }

    OS::cstdioWrapper *m_uut;
    OS::FileSystemInterface *m_fs;
    std::string m_testFile;
    std::string m_testFileContents;
    OS::Date m_date;
    std::string m_testOutput;
};

TEST(cstdioWrapper, readFilePass){
    OS::FILE_t *file = m_uut->fopen(m_testFile, std::string("r"));
    CHECK(file != NULL);
    std::string input;
    size_t i = 0;
    do{
        int rc = m_uut->fgetc(file);
        if (rc != OS::cstdioWrapper::END_OF_FILE){
            CHECK_EQUAL(rc, m_testFileContents[i]);
        }
        CHECK_EQUAL(m_uut->ferror(file), 0);
        ++i;
    }while(m_uut->feof(file) == 0);
    m_uut->fclose(file);
}

TEST(cstdioWrapper, writeFilePass){
    std::string outputFile = m_testOutput + std::string("fputcTest.txt");
    OS::FILE_t *file = m_uut->fopen(outputFile, std::string("w"));
    CHECK(file != NULL);
    for(size_t i = 0; i < m_testFileContents.size(); ++i){
        CHECK_EQUAL(m_uut->fputc(m_testFileContents[i], file), m_testFileContents[i]);
        CHECK_EQUAL(m_uut->ferror(file), 0);
    }
    m_uut->fclose(file);

    //Open the file to ensure it works
    std::string input;
    std::fstream inFile(outputFile, std::fstream::in);
    CHECK(!inFile.fail());
    inFile >> input;
    inFile.close();
    CHECK_EQUAL(input, m_testFileContents);
}

TEST(cstdioWrapper, fopenFail){
    OS::FILE_t *file = m_uut->fopen(std::string("DERP"), std::string("r"));
    CHECK(file == NULL);
}

TEST(cstdioWrapper, fopenEmptyString){
    OS::FILE_t *file = m_uut->fopen(std::string(""), std::string("r"));
    CHECK(file == NULL);
}

TEST(cstdioWrapper, renameSuccessTest){
    std::string renameFile = m_testOutput + std::string("renameTest1.txt");
    std::string newFile =  m_testOutput + std::string("renamedFile.txt");
    std::fstream outFile(renameFile, std::fstream::out);
    outFile << m_testFileContents;
    outFile.close();
    CHECK(m_fs->fileExists(renameFile));
    CHECK(!m_fs->fileExists(newFile));

    CHECK_EQUAL(m_uut->rename(renameFile, newFile), 0);
    CHECK(!m_fs->fileExists(renameFile));
    CHECK(m_fs->fileExists(newFile));
}

TEST(cstdioWrapper, renameFailTest){
   CHECK(m_uut->rename(std::string("DERP"), std::string("herp")) != 0);
}

TEST(cstdioWrapper, removeSuccessTest){
    std::string newFile = m_testOutput + std::string("removeTest");
    std::fstream outFile(newFile, std::fstream::out);
    outFile << m_testFileContents;
    outFile.close();

    CHECK(m_fs->fileExists(newFile));
    CHECK_EQUAL(m_uut->remove(newFile), 0);
    CHECK(!m_fs->fileExists(newFile));
}

TEST(cstdioWrapper, removeFailTest){
    CHECK(m_uut->remove(std::string("DERP")) != 0);
}

#endif

