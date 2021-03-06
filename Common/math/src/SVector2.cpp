
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cfloat>
#include <cmath>

#include "Operations.h"
#include "SVector2.h"

namespace Math{

SVector2 SVector2::fromPolar(const float &magnitude, const float &angleRadians){
    return Operations::polarToCartesian(magnitude, angleRadians);
}

SVector2::SVector2() :
    m_x(0),
    m_y(0)
{
}

SVector2::SVector2(const float &x, const float &y) :
    m_x(x),
    m_y(y)
{
}

SVector2::~SVector2(){
}

void SVector2::setX(const float &x){
    m_x = x;
}

void SVector2::setY(const float &y){
    m_y = y;
}

void SVector2::setZero(){
    m_x = 0;
    m_y = 0;
}

void SVector2::set(const float &x, const float &y){
    m_x = x;
    m_y = y;
}

void SVector2::setFromPolar(const float &radius, const float &angle){
    m_x = radius * std::cos(angle);
    m_y = radius * std::sin(angle);
}

void SVector2::setNegate(){
    m_x = -m_x;
    m_y = -m_y;
}

float SVector2::getX() const{
    return m_x;
}

float SVector2::getY() const{
    return m_y;
}

SVector2 SVector2::operator-() const{
    return SVector2(-m_x, -m_y);
}

void SVector2::operator+=(const SVector2 &other){
    m_x += other.m_x;
    m_y += other.m_y;
}

void SVector2::operator-=(const SVector2 &other){
    m_x -= other.m_x;
    m_y -= other.m_y;
}

void SVector2::operator*=(const float &f){
    m_x *= f;
    m_y *= f;
}

void SVector2::operator/=(const float &f){
    m_x /= f;
    m_y /= f;
}

SVector2 SVector2::operator+(const SVector2 &other) const{
    return SVector2(m_x + other.m_x, m_y + other.m_y);
}

SVector2 SVector2::operator-(const SVector2 &other) const{
    return SVector2(m_x - other.m_x, m_y - other.m_y);
}

SVector2 SVector2::operator*(const float &f) const{
    return SVector2(m_x * f, m_y * f);
}

SVector2 SVector2::operator/(const float &f) const{
    return SVector2(m_x / f, m_y / f);
}

float SVector2::dot(const SVector2 &other) const{
    return (m_x * other.m_x) + (m_y * other.m_y);
}

float SVector2::getMagnitudeSquared() const{
    return Operations::magnitudeSquared(m_x, m_y);
}

float SVector2::getMagnitude() const{
    return Operations::magnitude(m_x, m_y);
}

void SVector2::normalize(){
    float mag = getMagnitude();
    if (mag >= FLT_EPSILON){ //NO OP if zero.
        m_x = m_x / mag;
        m_y = m_y / mag;
    }
}

void SVector2::setMagnitude(const float &mag){
    normalize();
    (*this) *= mag;
}

void SVector2::limit(const float &lim){
    if ((getMagnitudeSquared()) > (lim * lim)){
        setMagnitude(lim);
    }
}

float SVector2::getHeading() const{
    return -1.0f * (std::atan2(-m_y, m_x));
}

void SVector2::rotate(const float &angle){
    float xTemp = m_x;
    m_x = (xTemp * std::cos(angle)) - (m_y * std::sin(angle));
    m_y = (xTemp * std::sin(angle)) + (m_y * std::cos(angle));
}

}

