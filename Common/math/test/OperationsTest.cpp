#include <cmath>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "Constants.h"
#include "Operations.h"

TEST_GROUP(Operations){
    float x = 10;
    float y = 20;
    float z = 30;
};

TEST(Operations, magnitudeSquaredTest2){
    float ans = (x * x) + (y * y);
    DOUBLES_EQUAL(ans, Math::Operations::magnitudeSquared(x, y), 0.001);
}

TEST(Operations, magnitudeSquaredTest3){
    float ans = (x * x) + (y * y) + (z * z);
    DOUBLES_EQUAL(ans, Math::Operations::magnitudeSquared(x, y, z), 0.001);
}

TEST(Operations, magnitudeTest2){
    float ans = std::sqrt((x * x) + (y * y));
    DOUBLES_EQUAL(ans, Math::Operations::magnitude(x, y), 0.001);
}

TEST(Operations, magnitudeTest3){
    float ans = std::sqrt((x * x) + (y * y) + (z * z));
    DOUBLES_EQUAL(ans, Math::Operations::magnitude(x, y, z), 0.001);
}

/// \details http://www.analyzemath.com/Calculators_2/convert_degrees_radians.html
TEST(Operations, RadianTest){
    float degrees = 90;
    DOUBLES_EQUAL(1.5707963, Math::Operations::radians(degrees), 0.0001);
}

TEST(Operations, DegreeTest){
    float radians = TWO_PI;
    DOUBLES_EQUAL(360, Math::Operations::degrees(radians), 0.0001);
}

TEST(Operations, polarToCartesianTestNoOffset){
    Math::SVector2 expected (7.071067, 7.071067);
    Math::SVector2 actual = Math::Operations::polarToCartesian(10.0f, Math::Operations::radians(45.0f));

    DOUBLES_EQUAL(expected.getX(), actual.getX(), 0.0001);
    DOUBLES_EQUAL(expected.getY(), actual.getY(), 0.0001);
}

TEST(Operations, polarToCartesianTestWithOffset){
    float xOffset = 2;
    float yOffset = 3;
    Math::SVector2 offset (xOffset, yOffset);
    Math::SVector2 expected (7.071067f + xOffset, 7.071067f + yOffset);
    Math::SVector2 actual = Math::Operations::polarToCartesian(10.0f, Math::Operations::radians(45.0f), offset);

    DOUBLES_EQUAL(expected.getX(), actual.getX(), 0.0001);
    DOUBLES_EQUAL(expected.getY(), actual.getY(), 0.0001);
}

TEST(Operations, midPointTest){
    float x1 = 10.0f;
    float y1 = 20.0f;
    float x2 = 30.0f;
    float y2 = 40.0f;
    Math::SVector2 expected(20.0f, 30.0f);

    Math::SVector2 actual = Math::Operations::midPoint(x1, y1, x2, y2);

    DOUBLES_EQUAL(expected.getX(), actual.getX(), 0.0001);
    DOUBLES_EQUAL(expected.getY(), actual.getY(), 0.0001);
}

