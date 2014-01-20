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

