#pragma once

#include <vector>

#include "pixel.hh"
#include "terrain_noise.hh"

class Chunk
{
public:
    Chunk(size_t pos_x, size_t pos_y, int size, Terrain_Noise terrain)
        : x_(pos_x)
        , y_(pos_y)
        , size_(size)
    {
        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                Vector3 a1 = Vector3(
                    x + 1, y, terrain.get_noise(pos_x + x + 1, pos_y + y));
                Vector3 a2 = Vector3(
                    x - 1, y, terrain.get_noise(pos_x + x - 1, pos_y + y));
                Vector3 b1 = Vector3(
                    x, y + 1, terrain.get_noise(pos_x + x, pos_y + y + 1));
                Vector3 b2 = Vector3(
                    x, y - 1, terrain.get_noise(pos_x + x, pos_y + y - 1));
                Vector3 a = (a2 - a1).normalized();
                Vector3 b = (b2 - b1).normalized();
                Vector3 normal = cross(a, b);
                Color grass(0, 154, 22);
                Color sand(248, 214, 114);
                Color stone(136, 140, 141);
                Color water(0, 84, 148);
                char noise = terrain.get_noise(pos_x + x, pos_y + y);
                Pixel p = Pixel(pos_x + x, pos_y + y, noise, grass,
                                normal.normalized());
                if (normal.length() > 0.9)
                    p = Pixel(pos_x + x, pos_y + y, noise, stone,
                              normal.normalized());
                if (noise <= 55)
                {
                    p = Pixel(pos_x + x, pos_y + y, noise, sand,
                              normal.normalized());
                }
                if (noise <= 50)
                {
                    p = Pixel(pos_x + x, pos_y + y, noise, water,
                              Vector3(0, 0, 1));
                }
                if ((pos_x == size * size && pos_y == size * size)
                    || (pos_x == 0 && pos_y == 0))
                {
                    p = Pixel(pos_x + x, pos_y + y, 0, Color(255, 0, 0),
                              Vector3(0, 0, 1));
                }
                pixels_.push_back(p);
            }
        }
    }

    Pixel get_pixel(size_t x, size_t y)
    {
        size_t rel_x = x - x_;
        size_t rel_y = y - y_;
        if (pixels_.size() != size_ * size_)
            return Pixel(0, 0, 0, Color(255, 0, 255), Vector3(0, 0, 1));

        if (rel_x > size_ || rel_x > size_)
        {
            // std::cout << "x: " << rel_x << " y: " << rel_y << std::endl;
            return Pixel(0, 0, 0, Color(255, 0, 255), Vector3(0, 0, 1));
        }
        return pixels_[rel_y * size_ + rel_x];
    }

private:
    size_t x_;
    size_t y_;
    size_t size_;
    std::vector<Pixel> pixels_;
};
