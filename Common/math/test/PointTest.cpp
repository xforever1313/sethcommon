
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>

#include "Point.h"

TEST_GROUP(Point){
    TEST_SETUP(){
        m_x = 1;
        m_y = 2;
        m_z = 3;
    }
    double m_x, m_y, m_z;
};

TEST(Point, DefaultConstructorTest){
    Math::Point uut;
    CHECK_EQUAL(uut.x, 0);
    CHECK_EQUAL(uut.y, 0);
}

TEST(Point, doubleConstructorTest){
    Math::Point uut(m_x, m_y);
    CHECK_EQUAL(uut.x, m_x);
    CHECK_EQUAL(uut.y, m_y);
    CHECK_EQUAL(uut.z, 0);
}

TEST(Point, tripleConstructorTest){
    Math::Point uut(m_x, m_y, m_z);
    CHECK_EQUAL(uut.x, m_x);
    CHECK_EQUAL(uut.y, m_y);
    CHECK_EQUAL(uut.z, m_z);
}
