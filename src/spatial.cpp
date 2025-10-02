#include <spatial.h>

namespace viewport{
    extern sf::CircleShape spatial_origin;
    extern sf::Text spatial_name;
};

Spatial::Spatial(string name, v2f pos, float angle, v2f scale, sf::Color debug_color): 
            Node(name), 
            pos(pos), 
            angle(angle), 
            scale(scale),
            debug_color(debug_color){
                updateTransform();
}

void Spatial::updateLocalTransform(){
    local_transform = sf::Transform().translate(pos).rotate(sf::degrees(angle)).scale(scale);
}

void Spatial::updateTransform(){
    updateLocalTransform();

    Spatial* spar = dynamic_cast<Spatial*>(parent.lock().get());
    if(!spar){
        global_transform = local_transform; return;
    }
    global_transform = spar->getGlobalTransform() * local_transform;
}

mat3 Spatial::getGlobalTransform(){
    return global_transform;
}

v2f Spatial::getGlobalPos(){
    return v2f(global_transform.getMatrix()[12],global_transform.getMatrix()[13]);
}

// 0  1  2  3
// 4  5  6  7
// 8  9  10 11
// 12 13 14 15

void Spatial::drawDebug(){
    viewport::spatial_name.setFillColor(debug_color);
    viewport::spatial_name.setString(name);
    viewport::spatial_origin.setOutlineColor(debug_color);
    viewport::instaDraw(&viewport::spatial_name, getGlobalTransform());
    viewport::instaDraw(&viewport::spatial_origin, getGlobalTransform());
    for(auto& i : children)
        i->drawDebug();
}