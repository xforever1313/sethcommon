
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>
#include <sstream>
#include <string>

#define private public
#define protected public

#include "io/InputReader.h"
#include "io/StringLogger.h"

TEST_GROUP(InputReaderTest) {
    TEST_SETUP() {

    }

    TEST_TEARDOWN() {

    }
};

TEST(InputReaderTest, PromptForInputTest) {
    std::stringstream expectedInput;
    expectedInput << "Hello world!";

    std::string expectedPromptMessage = "Say something!\n>";
    Common::IO::StringLogger actualPromptMessage;

    std::string actualInput = Common::IO::InputReader::PromptForInput(expectedPromptMessage, expectedInput, actualPromptMessage);

    CHECK_EQUAL(actualInput, expectedInput.str());
    CHECK_EQUAL(actualPromptMessage.getString(), expectedPromptMessage);
}

// Uncomment this to test with an actual console
/*
DISABLED_TEST(InputReaderTest, ConsoleTest) {
    std::string input = Common::IO::InputReader::PromptForInput("Give me input\n>");
    Common::IO::ConsoleLogger::out.writeLine(input);
}
*/
