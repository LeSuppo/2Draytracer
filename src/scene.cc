#include "scene.hh"

void Scene::update_chunks(int x, int y)
{
    int a = x / chunk_size_;
    int b = y / chunk_size_;
    int chunk_x = x_base_ / chunk_size_;
    int chunk_y = y_base_ / chunk_size_;
    if (a < chunk_x)
    {
        x_base_ = a * chunk_size_;
        for (size_t i = 0; i < chunk_number_y_; i++)
        {
            for (size_t j = chunk_number_x_ - 1; j >= 1; j--)
            {
                chunks_[i * chunk_number_x_ + j] =
                    chunks_[i * chunk_number_x_ + j - 1];
            }
        }
        for (size_t i = 0; i < chunk_number_y_; i++)
        {
            Chunk c = Chunk(x_base_, y_base_ + i * chunk_size_, chunk_size_,
                            terrain_);
            chunks_[i * chunk_number_x_] = c;
        }
    }
    else if (a > chunk_x)
    {
        x_base_ = a * chunk_size_;
        for (size_t i = 0; i < chunk_number_y_; i++)
        {
            for (size_t j = 0; j < chunk_number_x_ - 1; j++)
            {
                chunks_[i * chunk_number_x_ + j] =
                    chunks_[i * chunk_number_x_ + j + 1];
            }
        }
        for (size_t i = 0; i < chunk_number_y_; i++)
        {
            Chunk c = Chunk(x_base_ + (chunk_number_x_ - 1) * chunk_size_,
                            y_base_ + (i)*chunk_size_, chunk_size_, terrain_);
            chunks_[i * chunk_number_x_ + chunk_number_x_ - 1] = c;
        }
    }

    if (b < chunk_y)
    {
        y_base_ = b * chunk_size_;
        for (size_t i = 0; i < chunk_number_x_; i++)
        {
            for (size_t j = chunk_number_y_ - 1; j >= 1; j--)
            {
                chunks_[j * chunk_number_x_ + i] =
                    chunks_[(j - 1) * chunk_number_x_ + i];
            }
        }
        for (size_t i = 0; i < chunk_number_x_; i++)
        {
            Chunk c = Chunk(x_base_ + i * chunk_size_, y_base_, chunk_size_,
                            terrain_);
            chunks_[i] = c;
        }
    }
    else if (b > chunk_y)
    {
        y_base_ = b * chunk_size_;
        for (size_t i = 0; i < chunk_number_x_; i++)
        {
            for (size_t j = 0; j < chunk_number_y_ - 1; j++)
            {
                chunks_[(j)*chunk_number_x_ + i] =
                    chunks_[(j + 1) * chunk_number_x_ + i];
            }
        }
        for (size_t i = 0; i < chunk_number_x_; i++)
        {
            Chunk c = Chunk(x_base_ + i * chunk_size_,
                            y_base_ + (chunk_number_y_ - 1) * chunk_size_,
                            chunk_size_, terrain_);
            chunks_[(chunk_number_y_ - 1) * chunk_number_x_ + i] = c;
        }
    }
}

Pixel Scene::get_pixel(int x, int y) const
{
    size_t chunk_x = (x - x_base_) / chunk_size_;
    size_t chunk_y = (y - y_base_) / chunk_size_;
    if (chunk_x >= chunk_size_ * 2 || chunk_y >= chunk_size_ * 2)
    {
        return Pixel(0, 0, 0, Color(0, 0, 0), Vector3(0, 0, 1));
    }

    return chunks_[chunk_y * chunk_number_x_ + chunk_x].get_pixel(x, y);
}

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
