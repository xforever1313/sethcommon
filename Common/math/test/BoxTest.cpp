
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <CppUTest/TestHarness.h>

#include "Box.h"

TEST_GROUP(Box){
    TEST_SETUP(){
        m_x = 2;
        m_y = 3;
        m_z = 4;
        m_width = 5;
        m_height = 6;
        m_uut = new Math::Box(m_x, m_y, m_z, m_width, m_height);
    }

    TEST_TEARDOWN(){
        delete m_uut;
    }
    double m_x, m_y, m_z, m_width, m_height;
    Math::Box *m_uut;
};

TEST(Box, ConstructorTest){
    CHECK_EQUAL(m_uut->x, m_x);
    CHECK_EQUAL(m_uut->y, m_y);
    CHECK_EQUAL(m_uut->z, m_z);
    CHECK_EQUAL(m_uut->width, m_width);
    CHECK_EQUAL(m_uut->height, m_height);
}

TEST(Box, DefaultConstructorTest){
    Math::Box uut;

    CHECK_EQUAL(uut.x, 0);
    CHECK_EQUAL(uut.y, 0);
    CHECK_EQUAL(uut.z, 0);
    CHECK_EQUAL(uut.width, 0);
    CHECK_EQUAL(uut.height, 0);
}

TEST(Box, isInBoxTestInBox){
    CHECK(m_uut->isInBox(3, 4));
    CHECK(m_uut->isInBox(m_x, m_y));
    CHECK(m_uut->isInBox(m_x + m_width, m_y));
    CHECK(m_uut->isInBox(m_x + m_width, m_y + m_height));
    CHECK(m_uut->isInBox(m_x, m_y + m_height));
    CHECK(m_uut->isInBox(m_x + 2, m_y + 2));
}

TEST(Box, isInBoxTestOutOfBox){
    CHECK(!m_uut->isInBox(-2, 0));
    CHECK(!m_uut->isInBox(m_x - 0.00001, m_y - 0.00001));
    CHECK(!m_uut->isInBox(m_x + m_width + 0.000001, m_y + 0.000001));
    CHECK(!m_uut->isInBox(m_x + m_width + 0.000001, m_y + m_height + 0.00001));
    CHECK(!m_uut->isInBox(m_x, m_y + m_height + 0.00001));
    CHECK(!m_uut->isInBox(m_x - 2, m_y - 2));

    //Top
    CHECK(!m_uut->isInBox(m_x, m_y - 2));

    //Bottom
    CHECK(!m_uut->isInBox(m_x, m_y + m_height + 2));

    //Left
    CHECK(!m_uut->isInBox(m_x - 2, m_y));

    //Right
    CHECK(!m_uut->isInBox(m_x + m_width + 2, m_y));
}
