#include <core/viewport.h>

namespace debug{
    sf::Font default_font("assets/main/fonts/courier.ttf");

    bool show_fps = 0,
         show_spatial_name = 0,
         show_spatial_origin = 0,
         show_picked_node_info = 0,
         show_sprite_bounds = 0,
         show_aabb_bounds = 0,
         show_grid = 0;

    v2f grid_size = {256.f, 256.f},
        grid_offset = {0.f, 0.f};

    sf::Text fps_text(default_font),
                node_info(default_font);
    sf::VertexArray axis_x(sf::PrimitiveType::LineStrip,5);
    sf::VertexArray axis_y(sf::PrimitiveType::LineStrip,5);

    sf::VertexArray line(sf::PrimitiveType::LineStrip,2);

    void init(){
        default_font.setSmooth(0);

        fps_text.move(v2f(16.f,16.f));
        fps_text.setCharacterSize(16);
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
    void showGrid(){
        show_grid = !show_grid;
    }
    void showAll(){
        showFps();
        showSpatialName();
        showSpatialOrigin();
        showPickedNodeInfo();
        showSpriteBounds();
        showAABBBounds();
        showGrid();
    }

    sf::Text *spatial_name(string &name, sf::Color &debug_color){
        sf::Text *result = new sf::Text(default_font);
        result->setCharacterSize(16);
        result->setString(name);
        result->setFillColor(debug_color);

        return result;
    }

    sf::RectangleShape *aabb_bounds(float &top, float &bottom, float &left, float &right,
        sf::Color &debug_color){
            sf::RectangleShape *result = new sf::RectangleShape(v2f(right-left,bottom-top));
            result->setOutlineColor(debug_color);
            result->setOutlineThickness(1.f);
            result->setFillColor(sf::Color::Transparent);
            result->move(v2f(left,top));

            return result;
        }
}

namespace viewport{
    v2u res;

    sf::RenderWindow wind;

    sf::Color bg_color;

    priority_queue<DrawInfo, vector<DrawInfo>, std::greater<DrawInfo>> draw_queue;

    v2f cam_pos = {-480.f,-360.f};
    float zoom = 1.f;

    void init(v2u r, string name){
        wind = sf::RenderWindow(sf::VideoMode(r),name);
        res = r;
        wind.setFramerateLimit(60);
    }

    v2f getResolutionFactor(){
        return v2f(wind.getSize()) / v2f(res);
    }
    v2f getInverseResolutionFactor(){
        return v2f(res) / v2f(wind.getSize());
    }

    mat3 getCamTransform(){
        return mat3().scale({zoom, zoom}).translate(-cam_pos);
    }

    v2f getGlobalCursorPos(){
        return v2f(sf::Mouse::getPosition(wind))\
                * getInverseResolutionFactor() / zoom\
                + cam_pos;
    }
    v2f getRelativeCursorPos(){
        return v2f(sf::Mouse::getPosition(wind))\
                * getInverseResolutionFactor() / zoom;
    }

    void display(float dt){
        // cout << "entered display func...\n";
        // cout << "clearing bg...\n";
        wind.clear(bg_color);
        // cout << "showing fps...\n";
        if(debug::show_fps){
            debug::fps_text.setString(std::to_string(1.0/dt));
            drawOver(&debug::fps_text, 0, 1);
        }
        // cout << "queue processing started...\n";
        zoom = std::min(std::max(zoom, 0.1f), 10.f);
        while(!draw_queue.empty()){
            DrawInfo curr = draw_queue.top();
            // cout << "trying to draw...  " << curr.d << '\n';
            mat3 transform = curr.transform;
            // cout << "trying to apply cam transform..." << '\n';
            if(!curr.follow_cam)
                transform = getCamTransform().combine(transform);
            // cout << "trying to draw like for real..." << '\n';
            
            sf::Sprite *spr = dynamic_cast<sf::Sprite*>(curr.d);
            if(spr && curr.rect != sf::IntRect()){
                sf::IntRect temp = spr->getTextureRect();
                spr->setTextureRect(curr.rect);
                wind.draw(*curr.d, sf::RenderStates(transform));
                spr->setTextureRect(temp);
            }
            else{
                wind.draw(*curr.d, sf::RenderStates(transform));
            }
                
            // cout << "trying to free..." << '\n';
            curr.freeD();
            // cout << "trying to pop..." << '\n';
            draw_queue.pop();
            // cout << "successful i guess." << '\n';
        }
        if(debug::show_grid){
            debug::line[0].color = debug::line[1].color = sf::Color(63, 63, 63);

            debug::line[0].position.x = 0;
            debug::line[1].position.x = res.x;
            for(
                float ys = (int)(cam_pos.y/debug::grid_size.y) *
                debug::grid_size.y + debug::grid_offset.y; 
                ys <= cam_pos.y + (res.y/zoom); 
                ys += debug::grid_size.y
            ){
                debug::line[0].position.y =
                debug::line[1].position.y = (ys-cam_pos.y) * zoom;
                wind.draw(debug::line);
            }

            debug::line[0].position.y = 0;
            debug::line[1].position.y = res.y;
            for(
                float xs = (int)(cam_pos.x/debug::grid_size.x) *
                debug::grid_size.x + debug::grid_offset.x; 
                xs <= cam_pos.x + (res.x/zoom); 
                xs += debug::grid_size.x
            ){
                debug::line[0].position.x =
                debug::line[1].position.x = (xs-cam_pos.x) * zoom;
                wind.draw(debug::line);
            }

            debug::line[0].color = debug::line[1].color = sf::Color::Red;

            debug::line[0].position.x = 0;
            debug::line[1].position.x = res.x;

            debug::line[0].position.y =
            debug::line[1].position.y = -cam_pos.y * zoom;
            wind.draw(debug::line);

            debug::line[0].color = debug::line[1].color = sf::Color::Green;

            debug::line[0].position.y = 0;
            debug::line[1].position.y = res.y;

            debug::line[0].position.x =
            debug::line[1].position.x = -cam_pos.x * zoom;
            wind.draw(debug::line);
        }
    }

    void draw(sf::Drawable *d, bool is_copy, bool follow, int z, mat3 transform, sf::IntRect rect){
        // cout << "trying to push draw queue...\n";
        draw_queue.push({d,is_copy, transform, z, follow, rect});
    }

    void drawOver(sf::Drawable *d, bool is_copy, bool follow, mat3 transform, sf::IntRect rect){
        // cout << "trying to draw over...\n";
        draw(d,is_copy,follow,debug_info_z,transform, rect);
    }

    void exit(){
        wind.close();
    }
}