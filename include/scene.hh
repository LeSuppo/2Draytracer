#pragma once

#include <cstring>
#include <memory>
#include <vector>

#include "camera.hh"
#include "chunk.hh"
#include "light.hh"
#include "ray.hh"
#include "terrain_noise.hh"

class Scene
{
public:
    Scene(Camera camera, double ns, double seed, size_t chunk_number_x,
          size_t chunk_number_y, int x_base, int y_base, size_t chunk_size)
        : light_(Light(1, Vector3(0, 0, -100)))
        , camera_(camera)
        , ns_(ns)
        , seed_(seed)
        , x_base_(x_base)
        , y_base_(y_base)
        , chunk_number_x_(chunk_number_x)
        , chunk_number_y_(chunk_number_y)
        , chunk_size_(chunk_size)
    {
        Terrain_Noise terrain = Terrain_Noise(seed);
        for (size_t y = 0; y < chunk_number_y; y++)
        {
            for (size_t x = 0; x < chunk_number_x; x++)
            {
                Chunk c = Chunk(x_base + x * chunk_size,
                                y_base + y * chunk_size, chunk_size, terrain);
                chunks_.push_back(c);
            }
        }
    }

    double ns() const
    {
        return ns_;
    }

    Pixel get_pixel(int x, int y)
    {
        size_t chunk_x = (x - x_base_) / chunk_size_;
        size_t chunk_y = (y - y_base_) / chunk_size_;
        if (chunk_x > chunk_size_ * 2 || chunk_y > chunk_size_ * 2)
        {
            return Pixel(0, 0, 0, Color(0, 0, 0), Vector3(0, 0, 1));
        }

        return chunks_[chunk_y * chunk_number_x_ + chunk_x].get_pixel(x, y);
    }

    Camera get_camera() const
    {
        return camera_;
    }

    void move_camera(Vector3 vec)
    {
        camera_.change_pos(vec);
    }

    Light get_sun()
    {
        return light_;
    }

    void move_sun(Vector3 vec)
    {
        light_.set_position(vec);
    }

private:
    std::vector<Chunk> chunks_;
    Light light_;
    Camera camera_;
    double ns_;
    double seed_;
    int x_base_;
    int y_base_;
    size_t chunk_number_x_;
    size_t chunk_number_y_;
    size_t chunk_size_;
};

Vector3 hit(Ray r);
