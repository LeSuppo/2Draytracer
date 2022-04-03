#pragma once

#include "vector3.hh"

class Light
{
public:
    Light(double intensity, Vector3 pos)
        : pos_(pos)
        , intensity_(intensity)
    {}

    double get_intensity()
    {
        return intensity_;
    }

    Vector3 get_pos()
    {
        return pos_;
    }

    void set_position(Vector3 vec)
    {
        pos_ = vec;
    }

private:
    Vector3 pos_;
    double intensity_;
};
