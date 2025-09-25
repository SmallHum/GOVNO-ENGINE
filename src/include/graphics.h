#pragma once

#include <config.h>

namespace viewport{
    struct DrawInfo{
        sf::Drawable *d;
        mat3 transform;
        int z;
    };

    static bool operator > (DrawInfo a, DrawInfo b){
            return a.z > b.z;
    }
    static bool operator < (DrawInfo a, DrawInfo b){
        return a.z < b.z;
    }

    extern v2i res;

    extern sf::RenderWindow wind;

    extern sf::Color bg_color;

    extern sf::Font default_font;

    extern bool show_fps,
         show_spatial_name,
         show_spatial_origin,
         show_picked_node_info,
         show_sprite_bounds,
         show_aabb_bounds;

    extern sf::CircleShape spatial_origin;
    extern sf::Text spatial_name,
                node_info;
    extern sf::RectangleShape sprite_bounds(),
                        aabb_bounds();

    extern priority_queue<DrawInfo> draw_queue;

    void init(v2u res, string name = "TEST WINDOW");

    void display();

    void pushDrawable(sf::Drawable *d, mat3 transform = mat3(), int z = 0);

    void exit();
};