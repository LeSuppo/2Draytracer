#pragma once

#include <memory>
#include <vector>

#include "ray.hh"
#include "utils.hh"
#include "vector3.hh"

class Object
{
public:
    virtual Vector3 get_position() = 0;

    virtual void change_pos(Vector3 pos) = 0;

    virtual void set_position(Vector3 pos) = 0;

protected:
    std::vector<std::shared_ptr<Object>> children_;
};
