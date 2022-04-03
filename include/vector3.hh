#pragma once

#include <cmath>
#include <iostream>

class Vector3
{
public:
    constexpr Vector3(double x, double y, double z) noexcept
        : x_(x)
        , y_(y)
        , z_(z)
    {}

    Vector3()
        : x_(0)
        , y_(0)
        , z_(0)
    {}

    double x() const
    {
        return x_;
    }

    double y() const
    {
        return y_;
    }

    double z() const
    {
        return z_;
    }

    double length() const
    {
        return sqrtf(x_ * x_ + y_ * y_ + z_ * z_);
    }

    double squaredNorm() const
    {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }

    Vector3 normalized() const
    {
        double k = 1.0f / sqrtf(x_ * x_ + y_ * y_ + z_ * z_);
        return Vector3(k * x_, k * y_, k * z_);
    }

    Vector3 sqrt() const
    {
        return Vector3(sqrtf(x_), sqrtf(y_), sqrtf(z_));
    }

    Vector3 operator-() const
    {
        return Vector3(-x_, -y_, -z_);
    }

private:
    double x_;
    double y_;
    double z_;
};

inline std::ostream &operator<<(std::ostream &os, const Vector3 &vec)
{
    return os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")"
              << std::endl;
}

inline Vector3 operator+(const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

inline Vector3 operator-(const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

inline Vector3 operator*(double t, const Vector3 &v)
{
    return Vector3(t * v.x(), t * v.y(), t * v.z());
}

inline Vector3 operator*(const Vector3 &v, double t)
{
    return Vector3(t * v.x(), t * v.y(), t * v.z());
}

inline Vector3 operator*(const Vector3 &lhs, const Vector3 &rhs)
{
    return Vector3(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}

inline Vector3 operator/(const Vector3 &v, double t)
{
    double k = 1.0f / t;
    return Vector3(k * v.x(), k * v.y(), k * v.z());
}

inline double squaredDistance(const Vector3 &lhs, const Vector3 &rhs)
{
    return (lhs.x() - rhs.x()) * (lhs.x() - rhs.x())
        + (lhs.y() - rhs.y()) * (lhs.y() - rhs.y())
        + (lhs.z() - rhs.z()) * (lhs.z() - rhs.z());
}

inline double distance(const Vector3 &lhs, const Vector3 &rhs)
{
    return sqrtf((lhs.x() - rhs.x()) * (lhs.x() - rhs.x())
                 + (lhs.y() - rhs.y()) * (lhs.y() - rhs.y())
                 + (lhs.z() - rhs.z()) * (lhs.z() - rhs.z()));
}

inline double dot(const Vector3 &lhs, const Vector3 &rhs)
{
    return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

inline Vector3 cross(const Vector3 &lhs, const Vector3 &rhs)
{
    double x = lhs.y() * rhs.z() - lhs.z() * rhs.y();
    double y = -(lhs.x() * rhs.z() - lhs.z() * rhs.x());
    double z = lhs.x() * rhs.y() - lhs.y() * rhs.x();
    return Vector3(x, y, z);
}
