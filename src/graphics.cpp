#include <graphics.h>

namespace viewport{
    v2i res;

    sf::RenderWindow wind;

    sf::Color bg_color;

    sf::Font default_font("assets/font/cour.ttf");

    bool show_fps = 0,
         show_spatial_name = 0,
         show_spatial_origin = 0,
         show_picked_node_info = 0,
         show_sprite_bounds = 0,
         show_aabb_bounds = 0;

    sf::CircleShape spatial_origin(4.f);
    sf::Text spatial_name(default_font),
                node_info(default_font);
    sf::RectangleShape sprite_bounds(),
                        aabb_bounds();

    priority_queue<DrawInfo> draw_queue;

    void init(v2u res, string name){
        wind = sf::RenderWindow(sf::VideoMode(res),name);
    }

    void display(){
        while(!draw_queue.empty()){
            DrawInfo curr = draw_queue.top();
            wind.draw(*curr.d, sf::RenderStates(curr.transform));
            draw_queue.pop();
        }
        wind.display();
        wind.clear(bg_color);
    }

    void pushDrawable(sf::Drawable *d, int z, mat3 transform){
        draw_queue.push({d, transform, z});
    }

    void instaDraw(sf::Drawable *d, mat3 transform){
        wind.draw(*d, sf::RenderStates(transform));
    }

    void exit(){
        wind.close();
    }
}