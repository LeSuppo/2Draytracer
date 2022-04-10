#include "camera.hh"

#include <iostream>

void Camera::rotate(double angle)
{
    double beta = degrees_to_radians(angle);

    double px = lower_left_corner_.x() - center_.x();
    double py = lower_left_corner_.y() - center_.y();

    double cx = px * cos(beta) - py * sin(beta);
    double cy = px * sin(beta) + py * cos(beta);
    double cz = lower_left_corner_.z();

    lower_left_corner_ = Vector3(cx + center_.x(), cy + center_.y(), cz);

    double hx = horizontal_.x() * cos(beta) - horizontal_.y() * sin(beta);
    double hy = horizontal_.x() * sin(beta) + horizontal_.y() * cos(beta);
    double hz = horizontal_.z();
    horizontal_ = Vector3(hx, hy, hz);

    double vx = vertical_.x() * cos(beta) - vertical_.y() * sin(beta);
    double vy = vertical_.x() * sin(beta) + vertical_.y() * cos(beta);
    double vz = horizontal_.z();
    vertical_ = Vector3(vx, vy, vz);
}

Camera::Camera(Vector3 center, double fov_v, double fov_h, double z_min)
{
    z_min_ = z_min;
    auto alpha = degrees_to_radians(fov_v);
    auto beta = degrees_to_radians(fov_h);

    double viewport_v = 2.0 * tan(alpha) * z_min;
    double viewport_h = 2.0 * tan(beta) * z_min;

    Vector3 focus = Vector3(center.x(), center.y(), 0);

    auto w = (focus - center).normalized();
    auto u = (cross(Vector3(0, 1, 0), w)).normalized();
    auto v = cross(w, u);

    center_ = center;
    horizontal_ = viewport_h * u;
    vertical_ = viewport_v * v;
    lower_left_corner_ = center - horizontal_ / 2 - vertical_ / 2 + w;
    std::cout << "center: " << center;
    std::cout << "left corner: " << lower_left_corner_;
    std::cout << "right corner: " << lower_left_corner_ + horizontal_;
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
