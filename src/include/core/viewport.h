#pragma once

#include <core/assets.h>

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

    extern sf::Text spatial_name,
                node_info;
    extern sf::RectangleShape sprite_bounds(),
                        aabb_bounds();
    extern sf::VertexArray axis_x;
    extern sf::VertexArray axis_y;

    extern priority_queue<DrawInfo> draw_queue;

    void init(v2u res, string name = "TEST WINDOW");

    void display(float dt);

    //Adds draw data to the queue, which is sorted by key Z
    void pushDrawable(sf::Drawable *d, int z = 0, mat3 transform = mat3());

    //Unlike pushDrawable, this function draws data on screen instantly.
    //Usually it's used to draw debug info.
    void instaDraw(sf::Drawable *d, mat3 transform = mat3());

    void showFps();
    void showSpatialName();
    void showSpatialOrigin();
    void showPickedNodeInfo();
    void showSpriteBounds();
    void showAABBBounds();

    void showAll();

    void exit();
};