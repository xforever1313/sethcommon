
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "Box.h"

namespace Math{

Box::Box() :
    x(0),
    y(0),
    z(0),
    width(0),
    height(0)
{
}

Box::Box(double x, double y, double z, double width, double height) :
    x(x),
    y(y),
    z(z),
    width(width),
    height(height)
{

}

Box::~Box(){
}

bool Box::isInBox(double otherX, double otherY){
    return (otherX >= x) && (otherX <= x + width) && (otherY >= y) && (otherY <= y + height);
}

}
