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
                char noise_ax = terrain.get_noise(pos_x + x - 1, pos_y + y);
                char noise_ay = terrain.get_noise(pos_x + x, pos_y + y - 1);
                char noise_bx = terrain.get_noise(pos_x + x + 1, pos_y + y);
                char noise_by = terrain.get_noise(pos_x + x, pos_y + y + 1);
                char dx = noise_ax - noise_bx;
                char dy = noise_ay - noise_by;
                Color c(0, 96, 0);
                Pixel p = Pixel(pos_x + x, pos_y + y, c,
                                Vector3(-dx, dy, 1).normalized());
                // p = Pixel(pos_x + x, pos_y + y, c, Vector3(0, 0,
                // 1).normalized()); p = Pixel(pos_x + x, pos_y + y, c,
                // Vector3(noise_ax, noise_ay, 1).normalized());
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
