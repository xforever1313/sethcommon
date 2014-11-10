#include <gmock/gmock.h>
#include <iostream>
#include <stdexcept>

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestPlugin.h"

int main(int argc, char** argv)
{
    int ret;
    try {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock(&argc, argv);
        ret = CommandLineTestRunner::RunAllTests(argc, argv);
    }
    catch (const std::exception &e) {
        std::cerr << "UNHANDLED EXCEPTION: \n" << e.what();
        ret = -1;
    }

    return ret;
}

