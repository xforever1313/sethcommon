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

float SVector2::magnitudeSquared() const{
    return Operations::magnitudeSquared(m_x, m_y);
}

float SVector2::magnitude() const{
    return Operations::magnitude(m_x, m_y);
}

}
