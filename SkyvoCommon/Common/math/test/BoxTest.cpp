#include <boost/test/unit_test.hpp>

#include "Box.h"

struct BoxFixture{
    BoxFixture() :
        m_x(2),
        m_y(3),
        m_z(4),
        m_width(5),
        m_height(6),
        m_uut(new Math::Box(m_x, m_y, m_z, m_width, m_height))
    {
    }

    virtual ~BoxFixture(){
        delete m_uut;
    }
    double m_x, m_y, m_z, m_width, m_height;
    Math::Box *m_uut;
};

BOOST_FIXTURE_TEST_SUITE(BoxTest, BoxFixture)

BOOST_AUTO_TEST_CASE(Box_ConstructorTest){
    BOOST_CHECK_EQUAL(m_uut->x, m_x);
    BOOST_CHECK_EQUAL(m_uut->y, m_y);
    BOOST_CHECK_EQUAL(m_uut->z, m_z);
    BOOST_CHECK_EQUAL(m_uut->width, m_width);
    BOOST_CHECK_EQUAL(m_uut->height, m_height);
}

BOOST_AUTO_TEST_CASE(Box_DefaultConstructorTest){
    Math::Box uut;

    BOOST_CHECK_EQUAL(uut.x, 0);
    BOOST_CHECK_EQUAL(uut.y, 0);
    BOOST_CHECK_EQUAL(uut.z, 0);
    BOOST_CHECK_EQUAL(uut.width, 0);
    BOOST_CHECK_EQUAL(uut.height, 0);
}

BOOST_AUTO_TEST_CASE(BOX_isInBoxTestInBox){
    BOOST_CHECK(m_uut->isInBox(3, 4));
    BOOST_CHECK(m_uut->isInBox(m_x, m_y));
    BOOST_CHECK(m_uut->isInBox(m_x + m_width, m_y));
    BOOST_CHECK(m_uut->isInBox(m_x + m_width, m_y + m_height));
    BOOST_CHECK(m_uut->isInBox(m_x, m_y + m_height));
    BOOST_CHECK(m_uut->isInBox(m_x + 2, m_y + 2));
}

BOOST_AUTO_TEST_CASE(BOX_isInBoxTestOutOfBox){
    BOOST_CHECK(!m_uut->isInBox(-2, 0));
    BOOST_CHECK(!m_uut->isInBox(m_x - 0.00001, m_y - 0.00001));
    BOOST_CHECK(!m_uut->isInBox(m_x + m_width + 0.000001, m_y + 0.000001));
    BOOST_CHECK(!m_uut->isInBox(m_x + m_width + 0.000001, m_y + m_height + 0.00001));
    BOOST_CHECK(!m_uut->isInBox(m_x, m_y + m_height + 0.00001));
    BOOST_CHECK(!m_uut->isInBox(m_x - 2, m_y - 2));

    //Top
    BOOST_CHECK(!m_uut->isInBox(m_x, m_y - 2));

    //Bottom
    BOOST_CHECK(!m_uut->isInBox(m_x, m_y + m_height + 2));

    //Left
    BOOST_CHECK(!m_uut->isInBox(m_x - 2, m_y));

    //Right
    BOOST_CHECK(!m_uut->isInBox(m_x + m_width + 2, m_y));
}


BOOST_AUTO_TEST_SUITE_END()
