#include <cmath>

#include "Constants.h"
#include "Operations.h"
#include "SVector2.h"

namespace Math{

float Operations::distanceSquared(const float &x, const float &y){
    return (x * x) + (y * y);
}

float Operations::distanceSquared(const float &x, const float &y, const float &z){
    return (x * x) + (y * y) + (z * z);
}

float Operations::distance(const float &x, const float &y){
    return std::sqrt(distanceSquared(x, y));
}

float Operations::distance(const float &x, const float &y, const float &z){
    return std::sqrt(distanceSquared(x, y, z));
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

}
