
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cfloat>
#include <CppUTest/TestHarness.h>
#include <sstream>

#define private public
#define protected public

#include "io/LoggerBase.h"

TEST_GROUP(LoggerBaseTest) {

    TEST_SETUP() {
        m_expectedSS = new std::stringstream;
        m_loggingSS = new std::stringstream;
        m_uut = new Common::IO::LoggerBase(*m_loggingSS);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_loggingSS;
        delete m_expectedSS;
    }

    std::stringstream *m_expectedSS;
    std::stringstream *m_loggingSS;
    Common::IO::LoggerBase *m_uut;

};

TEST(LoggerBaseTest, writeToLogTest) {
    std::string expected1 = "Hello";
    std::string expected2 = " world!";
    m_uut->write(expected1 + expected2);

    CHECK_EQUAL(m_loggingSS->str(), expected1 + expected2);
}

TEST(LoggerBaseTest, writeLineToLogTest) {
    std::string expected = "Hello world!";
    (*m_expectedSS) << expected << std::endl;

    m_uut->writeLine(expected);

    CHECK_EQUAL(m_loggingSS->str(), m_expectedSS->str());
}

TEST(LoggerBaseTest, toStringTest) {
    int a = 13;
    long b = 0x14;
    long long c = 15;
    unsigned int d = 16e3;
    unsigned long e = 17;
    unsigned long long f = 18;
    float g = FLT_EPSILON;
    double h = DBL_EPSILON;
    long double i = LDBL_EPSILON;
    bool j = true;

    // noboolalpha since std::to_string(bool) returns 1 or 0
    // to_string is added for the double values as this method just tests to make sure everything
    // is callable with to_string.
    (*m_expectedSS) << std::noboolalpha << a << b << c << d << e << f << std::to_string(g) << std::to_string(h) << std::to_string(i) << j;

    m_uut->write(std::to_string(a) + std::to_string(b) + std::to_string(c) + std::to_string(d) + \
                 std::to_string(e) + std::to_string(f) + std::to_string(g) + std::to_string(h) + \
                 std::to_string(i) + std::to_string(j));

    CHECK_EQUAL(m_loggingSS->str(), m_expectedSS->str());
}

/**
 * \brief this and the next test ensures that char arrays are concatenatable
 */
TEST(LoggerBaseTest, charArrayTest1) {
    char c [6] = "Hello";
    std::string s = " world!";

    (*m_expectedSS) << c << s;

    m_uut->write(c + s);

    CHECK_EQUAL(m_loggingSS->str(), m_expectedSS->str());
}

TEST(LoggerBaseTest, charArrayTest2) {
    std::string s = "Hello";
    char c [8] = " world!";

    (*m_expectedSS) << s << c;

    m_uut->write(s + c);

    CHECK_EQUAL(m_loggingSS->str(), m_expectedSS->str());
}

TEST(LoggerBaseTest, charArrayTest3) {
    char c [] = "4 - 3 = ";
    int i = 1;

    (*m_expectedSS) << c << i;

    m_uut->write(c + std::to_string(i));

    CHECK_EQUAL(m_loggingSS->str(), m_expectedSS->str());
}
