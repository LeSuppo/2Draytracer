#pragma once

#include <vector>

#include "pixel.hh"
#include "terrain_noise.hh"

class Chunk
{
public:
    Chunk(size_t pos_x, size_t pos_y, Terrain_Noise terrain)
        : x_(pos_x)
        , y_(pos_y)
    {
        for (int y = 0; y < 16; y++)
        {
            for (int x = 0; x < 16; x++)
            {
                Vector3 a1 = Vector3(
                    x + 1, y, terrain.get_noise(pos_x + x + 1, pos_y + y));
                Vector3 a2 = Vector3(
                    x - 1, y, terrain.get_noise(pos_x + x - 1, pos_y + y));
                Vector3 b1 = Vector3(
                    x, y + 1, terrain.get_noise(pos_x + x, pos_y + y + 1));
                Vector3 b2 = Vector3(
                    x, y - 1, terrain.get_noise(pos_x + x, pos_y + y - 1));
                Vector3 a = a2 - a1;
                Vector3 b = b2 - b1;
                Vector3 normal = -cross(a, b);
                Color grass(0, 96, 0);
                Color water(0, 0, 255);
                char noise = terrain.get_noise(pos_x + x, pos_y + y);
                Pixel p =
                    Pixel(pos_x + x, pos_y + y, grass, normal.normalized());
                if (noise <= 50)
                {
                    p = Pixel(pos_x + x, pos_y + y, water, Vector3(0, 0, 1));
                }
                if ((pos_x == 16 * 16 && pos_y == 16 * 16)
                    || (pos_x == 0 && pos_y == 0))
                {
                    p = Pixel(pos_x + x, pos_y + y, Color(255, 0, 0),
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
        if (pixels_.size() != 256)
            return Pixel(0, 0, Color(0, 0, 255), Vector3(0, 0, 1));

        if (rel_x > 16 || rel_x > 16)
        {
            // std::cout << "x: " << rel_x << " y: " << rel_y << std::endl;
            return Pixel(0, 0, Color(0, 0, 255), Vector3(0, 0, 1));
        }
        return pixels_[rel_y * 16 + rel_x];
    }

private:
    size_t x_;
    size_t y_;
    std::vector<Pixel> pixels_;
};
