#pragma once

#include "object.hh"
#include "ray.hh"
#include "utils.hh"
#include "vector3.hh"

class Camera : public Object
{
public:
    Camera(Vector3 center, double fov_v, double fov_h, double z_min);

    Ray get_ray(double x, double y) const;

    Vector3 get_position()
    {
        return center_;
    }

    void change_pos(Vector3 pos)
    {
        if ((center_ + pos).z() >= 0) //|| (center_ + pos).z() < -170)
        {
            return;
        }
        center_ = center_ + pos;
        lower_left_corner_ = lower_left_corner_ + pos;
        for (std::shared_ptr<Object> c : children_)
        {
            c->change_pos(pos);
        }
    }

    void set_position(Vector3 pos);

private:
    Vector3 center_;
    Vector3 horizontal_;
    Vector3 vertical_;
    Vector3 lower_left_corner_;

    double z_min_;
    double viewport_v_;
    double viewport_h_;
};
