#include <boost/test/unit_test.hpp>

#include "Point.h"

struct PointFixture{
    PointFixture() :
        m_x(1),
        m_y(2),
        m_z(3)
    {
    }

    virtual ~PointFixture(){
    }

    double m_x, m_y, m_z;
};

BOOST_FIXTURE_TEST_SUITE(PointTest, PointFixture)

BOOST_AUTO_TEST_CASE(Point_DefaultConstructorTest){
    Math::Point uut;
    BOOST_CHECK_EQUAL(uut.x, 0);
    BOOST_CHECK_EQUAL(uut.y, 0);
}

BOOST_AUTO_TEST_CASE(Point_doubleConstructorTest){
    Math::Point uut(m_x, m_y);
    BOOST_CHECK_EQUAL(uut.x, m_x);
    BOOST_CHECK_EQUAL(uut.y, m_y);
    BOOST_CHECK_EQUAL(uut.z, 0);
}

BOOST_AUTO_TEST_CASE(Point_tripleConstructorTest){
    Math::Point uut(m_x, m_y, m_z);
    BOOST_CHECK_EQUAL(uut.x, m_x);
    BOOST_CHECK_EQUAL(uut.y, m_y);
    BOOST_CHECK_EQUAL(uut.z, m_z);
}

BOOST_AUTO_TEST_SUITE_END()

