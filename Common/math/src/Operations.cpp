
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cmath>

#include "Constants.h"
#include "Operations.h"
#include "SVector2.h"

namespace Math{

float Operations::magnitudeSquared(const float &x, const float &y){
    return (x * x) + (y * y);
}

float Operations::magnitudeSquared(const float &x, const float &y, const float &z){
    return (x * x) + (y * y) + (z * z);
}

float Operations::magnitude(const float &x, const float &y){
    return std::sqrt(magnitudeSquared(x, y));
}

float Operations::magnitude(const float &x, const float &y, const float &z){
    return std::sqrt(magnitudeSquared(x, y, z));
}

float Operations::distanceSquared(const float &x1, const float &y1, const float &x2, const float &y2){
    return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}

float Operations::distanceSquared(const float &x1, const float &y1, const float &z1, 
                                  const float &x2, const float &y2, const float &z2){
    return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) + ((z1 - z2) * (z1 - z2));
}

float Operations::distance(const float &x1, const float &y1, const float &x2, const float &y2){
    return std::sqrt(distanceSquared(x1, y1, x2, y2));
}

float Operations::distance(const float &x1, const float &y1, const float &z1, 
                           const float &x2, const float &y2, const float &z2){
    return std::sqrt(distanceSquared(x1, y1, z1, x2, y2, z2));
}

float Operations::radians(const float &degrees){
    return (degrees * PI) / 180.0f;
}

float Operations::degrees(const float &radians){
    return (radians / PI) * 180.0f;
}

SVector2 Operations::polarToCartesian(const float &radius, const float &angleRadians){
    return SVector2(radius * std::cos(angleRadians), radius * std::sin(angleRadians));
}

SVector2 Operations::polarToCartesian(const float &radius, const float &angleRadians, const SVector2 &offset){
    return SVector2(radius * std::cos(angleRadians) + offset.getX(), radius * std::sin(angleRadians) + offset.getY());
}

SVector2 Operations::midPoint(const float &x1, const float &y1, const float &x2, const float &y2){
    return SVector2((x1 + x2) / 2.0f, (y1 + y2) / 2.0f);
}

}

