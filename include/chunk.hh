#pragma once

#include <vector>

#include "object.hh"
#include "pixel.hh"
#include "terrain_noise.hh"

class Chunk
{
public:
    Chunk(int pos_x, int pos_y, int size, Terrain_Noise terrain, size_t octave,
          double persistence, double scale);

    Pixel get_pixel(size_t x, size_t y) const;

    std::vector<std::shared_ptr<Object>> get_objects() const
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

private:
    int x_;
    int y_;
    size_t size_;
    size_t octave_;
    double persistence_;
    double scale_;
    std::vector<Pixel> pixels_;
    std::vector<std::shared_ptr<Object>> objects_;
};
