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
    Scene(Camera camera, double ns, double seed, size_t chunk_size_x,
          size_t chunk_size_y)
        : light_(Light(1, Vector3(0, 0, -10)))
        , camera_(camera)
        , ns_(ns)
        , seed_(seed)
        , chunk_size_x_(chunk_size_x)
        , chunk_size_y_(chunk_size_y)
    {
        Terrain_Noise terrain = Terrain_Noise(seed);
        for (size_t y = 0; y < 32; y++)
        {
            for (size_t x = 0; x < 32; x++)
            {
                Chunk c = Chunk(x * 16, y * 16, terrain);
                chunks_.push_back(c);
            }
        }
    }

    double ns() const
    {
        return ns_;
    }

    Pixel get_pixel(size_t x, size_t y)
    {
        // std::cout << "size_x: " << chunk_size_x_*32 << " size_y: " <<
        // chunk_size_y_ * 32 << std::endl;
        size_t chunk_x = x / 16;
        size_t chunk_y = y / 16;
        if (chunk_x > 32 || chunk_y > 32)
        {
            // std::cout << "x: " << chunk_x << " y: " << chunk_y << std::endl;
            return Pixel(0, 0, Color(0, 0, 0), Vector3(0, 0, 1));
        }

        return chunks_[chunk_y * chunk_size_x_ + chunk_x].get_pixel(x, y);
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
    size_t chunk_size_x_;
    size_t chunk_size_y_;
};

Vector3 hit(Ray r);
