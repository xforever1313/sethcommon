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

