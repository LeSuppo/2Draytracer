#include "camera.hh"

#include <iostream>

Camera::Camera(Vector3 center, double fov_v, double fov_h, double z_min)
{
    z_min_ = z_min;
    auto alpha = degrees_to_radians(fov_v);
    auto beta = degrees_to_radians(fov_h);

    double viewport_v = 2.0 * tan(alpha) * z_min;
    double viewport_h = 2.0 * tan(beta) * z_min;

    auto w = (Vector3(center.x(), center.y(), 0) - center).normalized();
    auto u = (cross(Vector3(0, 1, 0), w)).normalized();
    auto v = cross(w, u);

    center_ = center;
    horizontal_ = viewport_h * u;
    vertical_ = viewport_v * v;
    lower_left_corner_ = center - horizontal_ / 2 - vertical_ / 2 + w;
}

Ray Camera::get_ray(double x, double y) const
{
    return Ray(center_,
               (lower_left_corner_ + x * horizontal_ + y * vertical_)
                   - center_);
}

void Camera::set_position(Vector3 pos)
{
    Vector3 dist_to_screen = lower_left_corner_ - center_;
    center_ = pos;
    lower_left_corner_ = pos + dist_to_screen;
}
