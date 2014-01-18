#include "Point.h"

namespace Math{

Point::Point() :
    x(0),
    y(0),
    z(0)
{
}

Point::Point(double x, double y) :
    x(x),
    y(y),
    z(0)
{
}

Point::Point(double x, double y, double z) :
    x(x),
    y(y),
    z(z)
{
}

Point::~Point(){
}

}
