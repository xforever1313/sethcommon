
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>
#include <sstream>

#define private public
#define protected public

#include "io/StringLogger.h"

TEST_GROUP(StringLoggerTest) {
    TEST_SETUP() {
        m_uut = new Common::IO::StringLogger;
        CHECK(typeid(std::stringstream) == typeid(m_uut->m_ss));
    }

    TEST_TEARDOWN() {
        delete m_uut;
    }

    Common::IO::StringLogger *m_uut;
};

TEST(StringLoggerTest, writeTest) {
    std::string expected = "Hello World!";

    m_uut->write(expected);

    CHECK_EQUAL(m_uut->getString(), expected);
}

TEST(StringLoggerTest, writeLineTest) {
    std::string expected = "HelloWorld";
    m_uut->writeLine(expected);

    CHECK_EQUAL(m_uut->getString(), expected + '\n');
}
