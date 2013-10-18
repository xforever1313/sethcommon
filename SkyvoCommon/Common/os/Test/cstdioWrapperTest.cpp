#include <boost/test/unit_test.hpp>
#include <fstream>
#include <string>

#include "cstdioWrapper.h"
#include "Date.h"
#include "FileSystem.h"

struct cstdioWrapperFixture{
    cstdioWrapperFixture() :
        m_uut(new SkyvoOS::cstdioWrapper()),
        m_fs(SkyvoOS::FileSystem::getInstance()),
        m_testFile( "../../../Test/testFile.txt"),
        m_testFileContents("test!"),
        //Date constructed automatically
        m_testOutput("../../../testOutput/cstdioWrapperTest/cstdioWrapperTest/" + m_date.getFullDateUnderscores())
    {
    }

    virtual ~cstdioWrapperFixture(){
        delete m_uut;
    }

    SkyvoOS::cstdioWrapper *m_uut;
    SkyvoOS::FileSystemInterface *m_fs;
    std::string m_testFile;
    std::string m_testFileContents;
    SkyvoOS::Date m_date;
    std::string m_testOutput;
};

BOOST_FIXTURE_TEST_SUITE(cstdioWrapperTest, cstdioWrapperFixture)

BOOST_AUTO_TEST_CASE(cstdioWrapper_readFilePass_setUp){
    BOOST_CHECK(m_fs->createDir(m_testOutput));
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_readFilePass){
    SkyvoOS::FILE_t *file = m_uut->fopen(m_testFile, std::string("r"));
    BOOST_CHECK(file != NULL);
    std::string input;
    size_t i = 0;
    do{
        int rc = m_uut->fgetc(file);
        if (rc != SkyvoOS::cstdioWrapper::END_OF_FILE){
            BOOST_CHECK_EQUAL(rc, m_testFileContents[i]);
        }
        BOOST_CHECK_EQUAL(m_uut->ferror(file), 0);
        ++i;
    }while(m_uut->feof(file) == 0);
    m_uut->fclose(file);
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_writeFilePass){
    std::string outputFile = m_testOutput + std::string("fputcTest.txt");
    SkyvoOS::FILE_t *file = m_uut->fopen(outputFile, std::string("w"));
    BOOST_CHECK(file != NULL);
    for(size_t i = 0; i < m_testFileContents.size(); ++i){
        BOOST_CHECK_EQUAL(m_uut->fputc(m_testFileContents[i], file), m_testFileContents[i]);
        BOOST_CHECK_EQUAL(m_uut->ferror(file), 0);
    }
    m_uut->fclose(file);

    //Open the file to ensure it works
    std::string input;
    std::fstream inFile(outputFile, std::fstream::in);
    BOOST_CHECK(!inFile.fail());
    inFile >> input;
    inFile.close();
    BOOST_CHECK_EQUAL(input, m_testFileContents);
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_fopenFail){
    SkyvoOS::FILE_t *file = m_uut->fopen(std::string("DERP"), std::string("r"));
    BOOST_CHECK(file == NULL);
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_fopenEmptyString){
    SkyvoOS::FILE_t *file = m_uut->fopen(std::string(""), std::string("r"));
    BOOST_CHECK(file == NULL);
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_renameSuccessTest){
    std::string renameFile = m_testOutput + std::string("renameTest1.txt");
    std::string newFile =  m_testOutput + std::string("renamedFile.txt");
    std::fstream outFile(renameFile, std::fstream::out);
    outFile << m_testFileContents;
    outFile.close();
    BOOST_CHECK(m_fs->fileExists(renameFile));
    BOOST_CHECK(!m_fs->fileExists(newFile));

    BOOST_CHECK_EQUAL(m_uut->rename(renameFile, newFile), 0);
    BOOST_CHECK(!m_fs->fileExists(renameFile));
    BOOST_CHECK(m_fs->fileExists(newFile));
}

BOOST_AUTO_TEST_CASE(cstdioWraper_renameFailTest){
   BOOST_CHECK(m_uut->rename(std::string("DERP"), std::string("herp")) != 0);
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_removeSuccessTest){
    std::string newFile = m_testOutput + std::string("removeTest");
    std::fstream outFile(newFile, std::fstream::out);
    outFile << m_testFileContents;
    outFile.close();

    BOOST_CHECK(m_fs->fileExists(newFile));
    BOOST_CHECK_EQUAL(m_uut->remove(newFile), 0);
    BOOST_CHECK(!m_fs->fileExists(newFile));
}

BOOST_AUTO_TEST_CASE(cstdioWrapper_removeFailTest){
    BOOST_CHECK(m_uut->remove(std::string("DERP")) != 0);
}

BOOST_AUTO_TEST_SUITE_END()
