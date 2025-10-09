#include <core/viewport.h>

namespace viewport{
    v2i res;

    sf::RenderWindow wind;

    sf::Color bg_color;

    sf::Font default_font;

    bool show_fps = 0,
         show_spatial_name = 0,
         show_spatial_origin = 0,
         show_picked_node_info = 0,
         show_sprite_bounds = 0,
         show_aabb_bounds = 0;

    sf::Text fps_text(default_font),
             spatial_name(default_font),
                node_info(default_font);
    sf::RectangleShape sprite_bounds(),
                        aabb_bounds();
    sf::VertexArray axis_x(sf::PrimitiveType::LineStrip,5);
    sf::VertexArray axis_y(sf::PrimitiveType::LineStrip,5);

    priority_queue<DrawInfo> draw_queue;

    void init(v2u res, string name){
        wind = sf::RenderWindow(sf::VideoMode(res),name);
        wind.setFramerateLimit(60);

        default_font = *getFont("main:courier").lock();
        default_font.setSmooth(0);
        fps_text.move(v2f(16.f,16.f));
        fps_text.setCharacterSize(16);
        spatial_name.setCharacterSize(16);
        node_info.setCharacterSize(16);

        axis_x[1].position = v2f(16,0);
        axis_x[2].position = v2f(13,2);
        axis_x[3].position = v2f(13,-2);
        axis_x[4].position = v2f(16,0);

        axis_y[1].position = v2f(0,16);
        axis_y[2].position = v2f(2,13);
        axis_y[3].position = v2f(-2,13);
        axis_y[4].position = v2f(0,16);

        for(int i = 0; i < 5; i++){
            axis_x[i].color = sf::Color::Red;
            axis_y[i].color = sf::Color::Green;
        }
    }

    void display(float dt){
        while(!draw_queue.empty()){
            DrawInfo curr = draw_queue.top();
            wind.draw(*curr.d, sf::RenderStates(curr.transform));
            draw_queue.pop();
        }
        if(show_fps){
            fps_text.setString(std::to_string(1.0/dt));
            instaDraw(&fps_text);
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

    void instaDraw(sf::VertexArray *v, mat3 transform){
        wind.draw(*v, sf::RenderStates(transform));
    }

    void showFps(){
        show_fps = !show_fps;
    }
    void showSpatialName(){
        show_spatial_name = !show_spatial_name;
    }
    void showSpatialOrigin(){
        show_spatial_origin = !show_spatial_origin;
    }
    void showPickedNodeInfo(){
        show_picked_node_info = !show_picked_node_info;
    }
    void showSpriteBounds(){
        show_sprite_bounds = !show_sprite_bounds;
    }
    void showAABBBounds(){
        show_aabb_bounds = !show_aabb_bounds;
    }
    void showAll(){
        showFps();
        showSpatialName();
        showSpatialOrigin();
        showPickedNodeInfo();
        showSpriteBounds();
        showAABBBounds();
    }

    void exit(){
        wind.close();
    }
}