#include "chunk.hh"

#include <iostream>

Chunk::Chunk(int pos_x, int pos_y, int size, Terrain_Noise terrain)
    : x_(pos_x)
    , y_(pos_y)
    , size_(size)
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            Vector3 a1 = Vector3(x + 1, y + 1,
                                 -terrain.get_noise(pos_x + x + 1, pos_y + y));
            Vector3 a2 = Vector3(x - 1, y - 1,
                                 -terrain.get_noise(pos_x + x - 1, pos_y + y));
            Vector3 b1 = Vector3(x - 1, y + 1,
                                 -terrain.get_noise(pos_x + x, pos_y + y + 1));
            Vector3 b2 = Vector3(x + 1, y - 1,
                                 -terrain.get_noise(pos_x + x, pos_y + y - 1));
            Vector3 a = (a1 - a2).normalized();
            Vector3 b = (b1 - b2).normalized();
            Vector3 normal = cross(a, b);
            Color grass(0, 154, 22);
            Color sand(248, 214, 114);
            Color stone(136, 140, 141);
            Color snow(255, 250, 250);
            Color water(0, 84, 148);
            int noise = terrain.get_noise(pos_x + x, pos_y + y);
            Color c = grass;
            int water_level = 120;
            if (noise <= water_level + 5)
            {
                c = sand;
            }

            if (noise >= 200)
            {
                c = snow;
            }
            Pixel p =
                Pixel(pos_x + x, pos_y + y, noise, c, normal.normalized());
            if (noise <= water_level)
            {
                p = Pixel(pos_x + x, pos_y + y, noise, water, Vector3(0, 0, 1));
            }
            else
            {
                if (p.get_slope() > 0.7)
                {
                    // p = Pixel(pos_x + x, pos_y + y, noise, stone,
                    //           normal.normalized());
                }
            }
            pixels_.push_back(p);
        }
    }
}

Pixel Chunk::get_pixel(size_t x, size_t y) const
{
    size_t rel_x = x - x_;
    size_t rel_y = y - y_;
    if (pixels_.size() != size_ * size_)
        return Pixel(0, 0, 0, Color(255, 0, 255), Vector3(0, 0, 1));

    size_t i = rel_y * size_ + rel_x;
    if (i >= size_ * size_)
    {
        // std::cout<< "size: " << size_ * size_ << " i: " << i << std::endl;
        // std::cout << "x: " << rel_x << " y: " << rel_y << std::endl;
        return Pixel(0, 0, 0, Color(255, 255, 0), Vector3(0, 0, 1));
    }
    return pixels_[i];
}
