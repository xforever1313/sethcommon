
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>
#include <memory>
#include <fstream>
#include <string>

#define private public
#define protected public

#include "io/FileLogger.h"
#include "FileSystem.h"

const std::string fileLoggerTestOutput = "../../../../testOutput/FileLoggerTest";
unsigned int fileLoggerTestNumber = 0;
bool fileLoggerFirstRun = true; ///< True if this is the first time this fixture has run.

TEST_GROUP(FileLoggerTest) {
    void FIXTURE_SETUP() {
        fileLoggerFirstRun = false;

        CHECK(m_fs->createDir(fileLoggerTestOutput));
        CHECK(m_fs->dirExists(fileLoggerTestOutput));
    }

    TEST_SETUP() {
        m_fs = OS::FileSystem::getInstance();

        if (fileLoggerFirstRun) {
            FIXTURE_SETUP();
        }

        m_currentTestFile = OS::FileSystem::pathJoin(fileLoggerTestOutput,
                                                     "FileLoggerTest" + std::to_string(fileLoggerTestNumber++));
        m_uut = Common::IO::FileLogger::createFileLogger(m_currentTestFile);
        //CHECK(typeid(std::ofstream) == typeid(m_uut->m_fs));
        CHECK(m_uut->m_isOpen);
    }

    TEST_TEARDOWN() {
        m_uut->close();
        CHECK(!m_uut->m_isOpen);
    }

    std::string readFile() {
        std::string fileContents;
        CHECK(m_fs->readFile(fileContents, m_currentTestFile));

        return fileContents;
    }

    OS::FileSystemInterface *m_fs;
    std::string m_currentTestFile;
    std::shared_ptr<Common::IO::FileLogger> m_uut;
};

TEST(FileLoggerTest, writeTest) {
    std::string expected = "Hello World!";
    m_uut->write(expected);

    CHECK_EQUAL(readFile(), expected);
}

TEST(FileLoggerTest, writeLineTest) {
    std::string expected = "HelloWorld";
    m_uut->writeLine(expected);

    CHECK_EQUAL(readFile(), expected + '\n');
}

TEST(FileLoggerTest, openFailure) {
    try {
        std::shared_ptr<Common::IO::FileLogger> uut = Common::IO::FileLogger::createFileLogger("this/dir/does/not/exist.txt");
        FAIL ("Did not expect to open file");
    }
    catch (const std::runtime_error &) {
        //Pass
    }
}
