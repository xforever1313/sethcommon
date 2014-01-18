#include <gmock/gmock.h>

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestPlugin.h"

int main(int argc, char** argv)
{
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);
    return CommandLineTestRunner::RunAllTests(argc, argv);
}

