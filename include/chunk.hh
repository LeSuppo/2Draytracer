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
        for (size_t y = 0; y < 16; y++)
        {
            for (size_t x = 0; x < 16; x++)
            {
                char noise = terrain.get_noise(pos_x + x, pos_y + y);
                Color c(noise, noise, noise);
                Pixel p = Pixel(0, 0, c, Vector3(0, 0, 1));
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
            return pixels_[0];
        }
        return pixels_[rel_y * 16 + rel_x];
    }

private:
    size_t x_;
    size_t y_;
    std::vector<Pixel> pixels_;
};
