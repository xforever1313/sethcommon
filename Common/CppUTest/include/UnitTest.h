
//          Copyright Seth Hendrick 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ./LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
 * \file UnitTest.h
 * \brief Include this file for any unit tests so the correct
 *        files will be included.
 */
#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include <gmock/gmock.h>
#undef FAIL
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#endif

