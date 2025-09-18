#pragma once

#include <node.h>

struct Spatial : Node{
    mat3 transform;

    v2f pos;
    float angle;
    v2f scale;
};