#ifndef SVECTOR2_H
#define SVECTOR2_H

namespace Math{

class SVector2{
    public:

        /// \brief Construct a vector whose X and Y are both zero
        SVector2();

        SVector2(const float &x, const float &y);

        virtual ~SVector2();

        void setX(const float &x);
        void setY(const float &y);

        /// \brief sets both X and Y to zero.
        void setZero();
        void set(const float &x, const float &y);

        ///\brief negates both x and y of THIS vector.
        void setNegate();

        float getX() const;
        float getY() const;

        /// \return the opposite of this vector (both x and y are negative)
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

    private:
        float m_x;
        float m_y;
};

}

#endif
