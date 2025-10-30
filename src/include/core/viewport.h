#pragma once

#include <config.h>

struct DrawInfo{
    sf::Drawable *d;
    bool is_copy;
    mat3 transform;
    int z;
    bool follow_cam;

    void freeD(){
        if(!is_copy)return;
        delete d;
    }
};

static bool operator > (DrawInfo a, DrawInfo b){
        return a.z > b.z;
}
static bool operator < (DrawInfo a, DrawInfo b){
    return a.z < b.z;
}

namespace debug{
    extern sf::Font default_font;

    extern bool show_fps,
         show_spatial_name,
         show_spatial_origin,
         show_picked_node_info,
         show_sprite_bounds,
         show_aabb_bounds;

    extern sf::Text fps_text,
                node_info;
    extern sf::VertexArray axis_x;
    extern sf::VertexArray axis_y;

    void init();

    void showFps();
    void showSpatialName();
    void showSpatialOrigin();
    void showPickedNodeInfo();
    void showSpriteBounds();
    void showAABBBounds();
    void showAll();

    sf::Text *spatial_name(string &name, sf::Color &debug_color);
    sf::RectangleShape *aabb_bounds(float &top, float &bottom, float &left, float &right,
        sf::Color &debug_color);
};

namespace viewport{

    extern v2u res;
    extern sf::RenderWindow wind;

    extern sf::Color bg_color;

    extern priority_queue<DrawInfo> draw_queue;

    extern v2f cam_pos;

    void init(v2u res, string name = "TEST WINDOW");

    void display(float dt);

    // A regular drawing function.
    // Draws any ```sf::Drawable``` accounting depth value ```z```
    void draw(sf::Drawable *d, bool is_copy, bool follow = 0, int z = 0, mat3 transform = mat3());

    // A drawing function that draws over EVERYTHING
    // Usually it's used for debug info.
    void drawOver(sf::Drawable *d, bool is_copy, bool follow = 0, mat3 transform = mat3());

    void exit();
};