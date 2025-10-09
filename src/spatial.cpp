#include <structs/spatial.h>

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
                Node::icon = (unsigned char)192;
}

unsigned char Spatial::getStructId(){
    return 1;
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

void Spatial::drawDebug(){
    Node::drawDebug();
    viewport::spatial_name.setFillColor(debug_color);
    viewport::spatial_name.setString(name);
    if(viewport::show_spatial_name)
        viewport::instaDraw(&viewport::spatial_name, mat3().translate(getGlobalPos()));
    if(viewport::show_spatial_origin){
        viewport::instaDraw(&viewport::axis_x, global_transform);
        viewport::instaDraw(&viewport::axis_y, global_transform);
    }
}

void Spatial::process(){
    updateTransform();
    Node::process();
}

void Spatial::printInfo(std::ostream& s){
    Node::printInfo(s);
    s << "Spatial:\n" <<
    " pos: " << pos << '\n' <<
    " angle: " << angle << '\n' <<
    " scale: " << scale << '\n';
}