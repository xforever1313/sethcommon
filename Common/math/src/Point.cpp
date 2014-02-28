
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
