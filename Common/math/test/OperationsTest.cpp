
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cmath>
#include <CppUTest/TestHarness.h>

#define private public
#define protected public

#include "Constants.h"
#include "Operations.h"

TEST_GROUP(Operations){
    float x = 10;
    float x2 = 15;
    float y = 20;
    float y2 = 25;
    float z = 30;
    float z2 = 35;
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

///\details http://en.wikipedia.org/wiki/Distance
TEST(Operations, distanceSquaredTest2){
    float ans = ((x - x2) * (x - x2)) + ((y - y2) * (y - y2));
    DOUBLES_EQUAL(ans, Math::Operations::distanceSquared(x, y, x2, y2), 0.0001);
}

TEST(Operations, distanceSquaredTest3){
    float ans = ((x - x2) * (x - x2)) + ((y - y2) * (y - y2)) + ((z - z2) * (z - z2));
    DOUBLES_EQUAL(ans, Math::Operations::distanceSquared(x, y, z, x2, y2, z2), 0.0001);
}

TEST(Operations, distanceTest2){
    float ans = ((x - x2) * (x - x2)) + ((y - y2) * (y - y2));
    ans =std::sqrt(ans);
    DOUBLES_EQUAL(ans, Math::Operations::distance(x, y, x2, y2), 0.0001);
}

TEST(Operations, distanceTest3){
    float ans = ((x - x2) * (x - x2)) + ((y - y2) * (y - y2)) + ((z - z2) * (z - z2));
    ans = std::sqrt(ans);
    DOUBLES_EQUAL(ans, Math::Operations::distance(x, y, z, x2, y2, z2), 0.0001);
}

/// \details http://www.analyzemath.com/Calculators_2/convert_degrees_radians.html
TEST(Operations, RadianTest){
    float degrees = 90;
    DOUBLES_EQUAL(1.5707963, Math::Operations::radians(degrees), 0.0001);
}

TEST(Operations, DegreeTest){
    float radians = Math::TWO_PI;
    DOUBLES_EQUAL(360, Math::Operations::degrees(radians), 0.0001);
}

TEST(Operations, polarToCartesianTestNoOffset){
    Math::SVector2 expected (7.071067f, 7.071067f);
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

