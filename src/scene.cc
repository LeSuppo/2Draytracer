#include "scene.hh"

Vector3 hit(Ray ray)
{
    Vector3 d = ray.direction();
    double denominator = dot(d, Vector3(0, 0, 1));
    if (abs(denominator) < (2 ^ -52))
    {
        std::cout << "direction and plane parallel, no intersection"
                  << std::endl;
        return Vector3(0, 0,
                       0); // direction and plane parallel, no intersection
    }

    double t =
        dot(Vector3(0, 0, 0) - ray.origin(), Vector3(0, 0, 1)) / denominator;
    if (t < 0)
    {
        std::cout << " plane behind ray's origin" << std::endl;
        return Vector3(0, 0, 0); // plane behind ray's origin
    }

    return ray.origin() + ray.direction() * t;
}
