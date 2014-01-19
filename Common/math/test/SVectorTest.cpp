#include <CppUTest/TestHarness.h>

#define private public
#define protected public

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
