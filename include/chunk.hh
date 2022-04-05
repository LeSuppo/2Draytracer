#pragma once

#include <vector>

#include "object.hh"
#include "pixel.hh"
#include "terrain_noise.hh"

class Chunk
{
public:
    Chunk(size_t pos_x, size_t pos_y, int size, Terrain_Noise terrain);

    Pixel get_pixel(size_t x, size_t y);

    std::vector<std::shared_ptr<Object>> get_objects()
    {
        return objects_;
    }

    void add_object(std::shared_ptr<Object> obj)
    {
        objects_.push_back(obj);
    }

    void remove_object(std::shared_ptr<Object> obj)
    {
        remove(objects_.begin(), objects_.end(), obj);
    }

    std::shared_ptr<Chunk> get_chunk(size_t x, size_t y);

private:
    size_t x_;
    size_t y_;
    size_t size_;
    std::vector<Pixel> pixels_;
    std::vector<std::shared_ptr<Object>> objects_;
};
