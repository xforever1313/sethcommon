#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "Operations.h"
#include "SVector2.h"

TEST_GROUP(SVector2){

    TEST_SETUP(){
        m_uut = new Math::SVector2(m_x, m_y);
        m_other = new Math::SVector2(m_x2, m_y2);
        CHECK_EQUAL(m_uut->m_x, m_x);
        CHECK_EQUAL(m_uut->getX(), m_x);
        CHECK_EQUAL(m_uut->m_y, m_y);
        CHECK_EQUAL(m_uut->getY(), m_y);
    }

    TEST_TEARDOWN(){
        delete m_other;
        delete m_uut;
    }

    float m_x = 10;
    float m_y = 20;
    float m_x2 = 100;
    float m_y2 = 200;
    Math::SVector2 *m_uut;
    Math::SVector2 *m_other;
};

TEST(SVector2, fromPolarTest){
    float radius = 20;
    float angle = 135; //degrees
    Math::SVector2 expected (-14.142135f, 14.142135f);

    Math::SVector2 actual = Math::SVector2::fromPolar(radius, Math::Operations::radians(angle));

    DOUBLES_EQUAL(expected.getX(), actual.getX(), 0.0001);
    DOUBLES_EQUAL(expected.getY(), actual.getY(), 0.0001);
}

TEST(SVector2, defaultConstructorTest){
    Math::SVector2 s;
    CHECK_EQUAL(s.m_x, 0);
    CHECK_EQUAL(s.getX(), 0);
    CHECK_EQUAL(s.m_y, 0);
    CHECK_EQUAL(s.getY(), 0);
}

TEST(SVector2, setXTest){
    m_uut->setX(m_x2);
    CHECK_EQUAL(m_uut->m_x, m_x2);
    CHECK_EQUAL(m_uut->getX(), m_x2);
}

TEST(SVector2, setYTest){
    m_uut->setY(m_y2);
    CHECK_EQUAL(m_uut->m_y, m_y2);
    CHECK_EQUAL(m_uut->getY(), m_y2);
}

TEST(SVector2, setZeroTest){
    m_uut->setZero();
    CHECK_EQUAL(m_uut->getX(), 0);
    CHECK_EQUAL(m_uut->getY(), 0);
}

TEST(SVector2, setTest){
    m_uut->set(m_x2, m_y2);
    CHECK_EQUAL(m_uut->getX(), m_x2);
    CHECK_EQUAL(m_uut->getY(), m_y2);
}

TEST(SVector2, setNegateTest){
    m_uut->setNegate();
    CHECK_EQUAL(m_uut->getX(), -m_x);
    CHECK_EQUAL(m_uut->getY(), -m_y);
}

TEST(SVector2, operatorMinusTest){
    Math::SVector2 s = -(*m_uut);
    CHECK_EQUAL(s.getX(), -m_x);
    CHECK_EQUAL(s.getY(), -m_y);
}

TEST(SVector2, operatorPlusEqualsTest){
    (*m_uut) += (*m_other);
    DOUBLES_EQUAL(m_x2 + m_x, m_uut->getX(), 0.001);
    DOUBLES_EQUAL(m_y2 + m_y, m_uut->getY(), 0.001);
}

TEST(SVector2, operatorMinusEqualsTest){
    (*m_uut) -= (*m_other);
    DOUBLES_EQUAL(m_x - m_x2, m_uut->getX(), 0.001);
    DOUBLES_EQUAL(m_y - m_y2, m_uut->getY(), 0.001);
}

TEST(SVector2, operatorTimesEqualsTest){
    float f = 10;
    (*m_uut) *= f;
    DOUBLES_EQUAL(m_x * f, m_uut->getX(), 0.001);
    DOUBLES_EQUAL(m_y * f, m_uut->getY(), 0.001);
}

TEST(SVector2, operatorDivideEqualsTest){
    float f = 10;
    (*m_uut) /= f;
    DOUBLES_EQUAL(m_x / f, m_uut->getX(), 0.001);
    DOUBLES_EQUAL(m_y / f, m_uut->getY(), 0.001);
}

TEST(SVector2, operatorPlusTest){
    Math::SVector2 result = (*m_uut) + (*m_other);

    CHECK_EQUAL(m_uut->getX(), m_x);
    CHECK_EQUAL(m_uut->getY(), m_y);
    CHECK_EQUAL(m_other->getX(), m_x2);
    CHECK_EQUAL(m_other->getY(), m_y2);

    DOUBLES_EQUAL(m_x + m_x2, result.getX(), 0.001);
    DOUBLES_EQUAL(m_y + m_y2, result.getY(), 0.001);
}

TEST(SVector2, operatorSubtractTest){
    Math::SVector2 result = (*m_uut) - (*m_other);

    CHECK_EQUAL(m_uut->getX(), m_x);
    CHECK_EQUAL(m_uut->getY(), m_y);
    CHECK_EQUAL(m_other->getX(), m_x2);
    CHECK_EQUAL(m_other->getY(), m_y2);

    DOUBLES_EQUAL(m_x - m_x2, result.getX(), 0.001);
    DOUBLES_EQUAL(m_y - m_y2, result.getY(), 0.001);
}

TEST(SVector2, operatorMultiplyTest){
    float f = 3;
    Math::SVector2 result = (*m_uut) * f;

    CHECK_EQUAL(m_uut->getX(), m_x);
    CHECK_EQUAL(m_uut->getY(), m_y);

    DOUBLES_EQUAL(m_x * f, result.getX(), 0.001);
    DOUBLES_EQUAL(m_y * f, result.getY(), 0.001);
}

TEST(SVector2, operatorDivideTest){
    float f = 3;
    Math::SVector2 result = (*m_uut) / f;

    CHECK_EQUAL(m_uut->getX(), m_x);
    CHECK_EQUAL(m_uut->getY(), m_y);

    DOUBLES_EQUAL(m_x / f, result.getX(), 0.001);
    DOUBLES_EQUAL(m_y / f, result.getY(), 0.001);
}

TEST(SVector2, dotTest){
    float f = m_uut->dot(*m_other);
    CHECK_EQUAL(m_uut->getX(), m_x);
    CHECK_EQUAL(m_uut->getY(), m_y);
    CHECK_EQUAL(m_other->getX(), m_x2);
    CHECK_EQUAL(m_other->getY(), m_y2);

    CHECK_EQUAL(f, (m_x * m_x2) + (m_y * m_y2));
}

TEST(SVector2, magnitudeSquaredTest){
    float f = m_uut->getMagnitudeSquared();
    DOUBLES_EQUAL(f, Math::Operations::magnitudeSquared(m_x, m_y), 0.001);
}

TEST(SVector2, magnitudeTest){
    float f = m_uut->getMagnitude();
    DOUBLES_EQUAL(f, Math::Operations::magnitude(m_x, m_y), 0.001);
}

TEST(SVector2, normalizeTest){
    m_uut->setX(3);
    m_uut->setY(4); //Create 3, 4, 5 right triangle
    m_uut->normalize();
    DOUBLES_EQUAL(3.0f / 5.0f, m_uut->getX(), 0.0001);
    DOUBLES_EQUAL(4.0f / 5.0f, m_uut->getY(), 0.0001);
    DOUBLES_EQUAL(m_uut->getMagnitude(), 1.0f, 0.0001);
}

TEST(SVector2, normalizeZeroTest){
    m_uut->setX(0);
    m_uut->setY(0);
    m_uut->normalize();
    CHECK_EQUAL(m_uut->getMagnitude(), 0);
}

TEST(SVector2, setMagnitudeTest){
    m_uut->setX(3);
    m_uut->setY(4); //Create 3, 4, 5 right triangle
    m_uut->setMagnitude(10.0f);
    DOUBLES_EQUAL(6.0f, m_uut->getX(), 0.0001);
    DOUBLES_EQUAL(8.0f, m_uut->getY(), 0.0001);
    DOUBLES_EQUAL(m_uut->getMagnitude(), 10.0f, 0.0001);
}

TEST(SVector2, setMagnitudeZeroTest){
    m_uut->setX(0);
    m_uut->setY(0);
    m_uut->setMagnitude(12);
    CHECK_EQUAL(m_uut->getMagnitude(), 0);
}

TEST(SVector2, overLimitTest){
    m_uut->setX(6);
    m_uut->setY(8);
    m_uut->limit(5);
    DOUBLES_EQUAL(3.0f, m_uut->getX(), 0.0001);
    DOUBLES_EQUAL(4.0f, m_uut->getY(), 0.0001);
    DOUBLES_EQUAL(m_uut->getMagnitude(), 5.0f, 0.0001);
}

TEST(SVector2, underLimitTest){
    m_uut->setX(6);
    m_uut->setY(8);
    m_uut->limit(20);
    CHECK_EQUAL(m_uut->getX(), 6);
    CHECK_EQUAL(m_uut->getY(), 8);
    DOUBLES_EQUAL(m_uut->getMagnitude(), 10.0f, 0.0001);
}

TEST(SVector2, headingTest){
    m_uut->setX(3);
    m_uut->setY(4); //Create 3, 4, 5 right triangle
    DOUBLES_EQUAL(0.9272952f, m_uut->getHeading(), 0.0001);

    m_uut->setX(-3);
    DOUBLES_EQUAL(2.214297f, m_uut->getHeading(), 0.0001);
}

