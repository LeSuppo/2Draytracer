#pragma once

#include "color.hh"
#include "vector3.hh"

class Pixel
{
public:
    Pixel(size_t x, size_t y, size_t z, Color color, Vector3 normal)
        : x_(x)
        , y_(y)
        , z_(z)
        , color_(color)
        , normal_(normal)
    {}

    Color get_color()
    {
        return color_;
    }

    Vector3 normal()
    {
        return normal_;
    }

private:
    size_t x_;
    size_t y_;
    size_t z_;
    Color color_;
    Vector3 normal_;
};
