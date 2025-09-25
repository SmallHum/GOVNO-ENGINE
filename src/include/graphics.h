#pragma once

#include <config.h>

struct DrawInfo{
    sf::Drawable *d;
    mat3 transform;
    int z;
};

namespace viewport{
    extern v2i res;

    sf::RenderWindow wind;

    extern bool show_fps,
         show_spatial_name,
         show_spatial_origin,
         show_picked_node_info,
         show_sprite_bounds,
         show_aabb_bounds,
         show_control_input_info;

    priority_queue<DrawInfo> draw_queue;

    void init(v2i res, string name = "TEST WINDOW");

    void display();
};