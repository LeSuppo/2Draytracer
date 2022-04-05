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
            Vector3 a1 =
                Vector3(x + 1, y, terrain.get_noise(pos_x + x + 1, pos_y + y));
            Vector3 a2 =
                Vector3(x - 1, y, terrain.get_noise(pos_x + x - 1, pos_y + y));
            Vector3 b1 =
                Vector3(x, y + 1, terrain.get_noise(pos_x + x, pos_y + y + 1));
            Vector3 b2 =
                Vector3(x, y - 1, terrain.get_noise(pos_x + x, pos_y + y - 1));
            Vector3 a = (a2 - a1).normalized();
            Vector3 b = (b2 - b1).normalized();
            Vector3 normal = cross(a, b);
            Color grass(0, 154, 22);
            Color sand(248, 214, 114);
            Color stone(136, 140, 141);
            Color snow(255, 250, 250);
            Color water(0, 84, 148);
            int noise = terrain.get_noise(pos_x + x, pos_y + y);
            Pixel p =
                Pixel(pos_x + x, pos_y + y, noise, grass, normal.normalized());
            if (p.get_slope() > 0.2)
            {
                p = Pixel(pos_x + x, pos_y + y, noise, stone,
                          normal.normalized());
            }
            if (noise <= 55)
            {
                p = Pixel(pos_x + x, pos_y + y, noise, sand,
                          normal.normalized());
            }
            if (noise <= 50)
            {
                p = Pixel(pos_x + x, pos_y + y, noise, water, Vector3(0, 0, 1));
            }

            if (noise >= 200)
            {
                p = Pixel(pos_x + x, pos_y + y, noise, snow,
                          normal.normalized());
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
