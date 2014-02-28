
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef POINT_H_
#define POINT_H_

namespace Math{

struct Point{
    Point();

    Point(double x, double y);

    Point(double x, double y, double z);

    virtual ~Point();

    double x, y, z;
};

}

#endif

