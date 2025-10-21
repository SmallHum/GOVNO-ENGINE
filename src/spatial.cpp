#include <structs/spatial.h>
#include <core/viewport.h>
#include <fstream_opers.h>

namespace viewport{
    extern sf::CircleShape spatial_origin;
    extern sf::Text spatial_name;
};

Spatial::Spatial(){}

Spatial::Spatial(
    string name, 
    v2f pos, 
    float angle, 
    v2f scale, 
    sf::Color debug_color
): 
    Node(name), 
    pos(pos), 
    angle(angle), 
    scale(scale),
    debug_color(debug_color)
{
        updateTransform();
}

StructId Spatial::getStructId(){
    return StructId::Spatial;
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

void Spatial::reader(fstream &stream){
    Node::reader(stream);

    pos = fstreamReadV2f(stream);
    angle = fstreamRead<float>(stream);
    scale = fstreamReadV2f(stream);
}
void Spatial::writer(fstream &stream){
    Node::writer(stream);

    fstreamWriteV2f(stream, pos);
    fstreamWrite<float>(stream, angle);
    fstreamWriteV2f(stream, scale);
}

void Spatial::printInfo(std::ostream& s){
    Node::printInfo(s);
    s << "Spatial:\n" <<
    " pos: " << pos << '\n' <<
    " angle: " << angle << '\n' <<
    " scale: " << scale << '\n';
}