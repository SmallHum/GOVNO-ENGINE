#pragma once

#include <node.h>

struct Spatial : Node{
    char icon = '*';

    mat3 local_transform;
    mat3 global_transform;

    v2f pos;
    float angle;
    v2f scale;

    inline void updateLocalTransform();

    inline void updateTransform();

    mat3 getGlobalTransform();

    v2f getGlobalPos();
};