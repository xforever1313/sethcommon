
//          Copyright Seth Hendrick 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SVECTOR2_H
#define SVECTOR2_H

namespace Math{

class SVector2{
    public:
        /// \brief creates an SVector based on the given polar coordiante
        static SVector2 fromPolar(const float &magnitude, const float &angleRadians);

        /// \brief Construct a vector whose X and Y are both zero
        SVector2();

        SVector2(const float &x, const float &y);

        virtual ~SVector2();

        void setX(const float &x);
        void setY(const float &y);

        /// \brief sets both X and Y to zero.
        void setZero();
        void set(const float &x, const float &y);

        /// \brief sets the X and Y coordinates based on the given polar coordinate.
        /// \param angle The angle in RADIANS
        void setFromPolar(const float &radius, const float &angle);

        ///\brief negates both x and y of THIS vector.
        void setNegate();

        float getX() const;
        float getY() const;

        /// \return the opposite of this vector (both x and y are negated)
        SVector2 operator-() const;

        /// \brief use vector addition to add another vector to THIS vector
        void operator += (const SVector2 &other);

        /// \brief use vector subtraction to subtract another vector from THIS vector
        void operator -=(const SVector2 &other);

        /// \brief Multiplies THIS vector by a scalar
        void operator *= (const float &f);

        /// \brief Divides THIS vector by a scalar
        /// \warning passing in zero will result in a division by zero, which is bad.
        void operator /= (const float &f);

        /// \brief performs vector addition on THIS vector and the other vector, and returns the result
        /// \note This vector and the other vector are not changed
        /// \note The operation is C = A + B, where A is This vector, and B is the other vector.
        SVector2 operator+ (const SVector2 &other) const;

        /// \brief performs vector subtraction on THIS vector and the other vector, and returns the result
        /// \note This vector and the other vector are not changed
        /// \note The operation is C = A - B, where A is This vector, and B is the other vector.
        SVector2 operator- (const SVector2 &other) const;

        /// \brief performs scalar multiplication on THIS vector and f, and returns the result
        /// \note This vector is not changed.
        SVector2 operator* (const float &f) const;

        /// \brief performs scalar division on THIS vector and f, and returns the result
        /// \note This vector is not changed.
        /// \warning Passing in Zero will result in a division by zero, which is bad.
        SVector2 operator/ (const float &f) const;

        /// \brief performs a dot product on THIS vector and other, and returns the result
        /// \note This vector and other are not changed
        float dot(const SVector2 &other) const;

        /// \return the square of the magnitude (length) of the vector
        float getMagnitudeSquared() const;

        /// \return the magnitude (length) of the vector.
        float getMagnitude() const;

        /// \brief sets THIS vector's magnitude to 1.  The direction stays the same.
        /// \note Nothing happens if the Vector's magnitude is zero.
        void normalize();

        ///\brief sets the magnitude of the vector to the given value.  Direction stays the same.
        /// \note Nothing happens if the Vector's magnitude is zero.
        void setMagnitude(const float &mag);

        /// \brief If the magnitude of the vector is greater than the passed in value, the magnitude
        /// Is set the the passed in value.
        /// \note you must call this every time you want to limit the vector.
        void limit(const float &lim);

        /// \brief rotates the Vector.
        /// \param angle The angle in RADIANS
        /// \note If you know the magnitude, and the resulting angle, consider using setFromPolar.
        ///       rotate() uses 4 sin/cos functions, and setFromPolar uses 2.
        void rotate(const float &angle);

        /// \brief returns the angle of rotation of the vector.
        /// \return The rotation angle in RADIANS
        float getHeading() const;

    private:
        float m_x;
        float m_y;
};

}

#endif
