#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "SVector2.h"

namespace Math{

class Operations{
    public:
        static float distanceSquared(const float &x, const float &y);
        static float distanceSquared(const float &x, const float &y, const float &z);

        static float distance(const float &x, const float &y);
        static float distance(const float &x, const float &y, const float &z);

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

    private:
        Operations();
        ~Operations();
};

}

#endif // OPERATIONS_H
