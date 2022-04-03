#pragma once

#include <iostream>

#include "vector3.hh"

class Ray
{
public:
    Ray(const Vector3 &origin, const Vector3 &direction)
        : origin_(origin)
        , direction_(direction)
    {}

    const Vector3 &origin() const
    {
        return origin_;
    }

    const Vector3 &direction() const
    {
        return direction_;
    }

    Vector3 pointAtParameter(double t) const
    {
        return origin_ + (t * direction_);
    }

private:
    Vector3 origin_;
    Vector3 direction_;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &ray)
{
    return os << "origin: " << ray.origin()
              << ", direction: " << ray.direction() << std::endl;
}
