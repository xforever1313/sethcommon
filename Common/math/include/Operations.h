
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "SVector2.h"

namespace Math{

class Operations{
    public:
        static float magnitudeSquared(const float &x, const float &y);
        static float magnitudeSquared(const float &x, const float &y, const float &z);

        static float magnitude(const float &x, const float &y);
        static float magnitude(const float &x, const float &y, const float &z);

        static float distanceSquared(const float &x1, const float &y1, const float &x2, const float &y2);
        static float distanceSquared(const float &x1, const float &y1, const float &z1, 
                                     const float &x2, const float &y2, const float &z2);
        static float distance(const float &x1, const float &y1, const float &x2, const float &y2);
        static float distance(const float &x1, const float &y1, const float &z1, 
                              const float &x2, const float &y2, const float &z2);

        /// \brief converts the given degrees to radians
        static float radians(const float &degrees);

        /// \brief converts the given radians to degrees
        static float degrees(const float &radians);

        /// \brief converts the given polar coordiante to cartesian
        /// \param angleRadians The Angle in RADIANS
        /// \return a vector of cartesian coordiantes
        /// \note This method assumes that the polar coordiante's center is at 0,0
        /// If it is not, use polarToCartesian with the offset param 
        static SVector2 polarToCartesian(const float &radius, const float &angleRadians);

        /// \brief converts the given offset polar coordiante to cartesian
        /// \param angleRadians The Angle in RADIANS
        /// \param offset the point relative to the origin where the center of the polar coordinate is
        /// \return a vector of cartesian coordiantes
        static SVector2 polarToCartesian(const float &radius, const float &angleRadians, const SVector2 &offset);

        static SVector2 midPoint(const float &x1, const float &y1, const float &x2, const float &y2);

    private:
        Operations();
        ~Operations();
};

}

#endif 

