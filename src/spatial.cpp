#include <structs/spatial.h>
#include <core/viewport.h>

Spatial::Spatial(): Node(){
    name = "Spatial";
    pos = {0.f, 0.f};
    angle = 0.f;
    scale = {1.f,1.f};
    debug_color = hsv(rand());
    updateTransform();
}

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
    if(debug::show_spatial_name){
        viewport::drawOver(debug::spatial_name(name, debug_color), 1, 0, mat3().translate(getGlobalPos()));
    }
    if(debug::show_spatial_origin){
        viewport::drawOver(&debug::axis_x, 0, 0, global_transform);
        viewport::drawOver(&debug::axis_y, 0, 0, global_transform);
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

// void Spatial::printInfo(std::ostream& s){
//     Node::printInfo(s);
//     s << "Spatial:\n" <<
//     " pos: " << pos << '\n' <<
//     " angle: " << angle << '\n' <<
//     " scale: " << scale << '\n';
// }

void Spatial::copy(weak_ptr<Node> node){
    Node::copy(node);
    if(auto n = node.lock()){
        auto spatial = dynamic_cast<Spatial*>(n.get());
        if(spatial){
            pos = spatial->pos;
            angle = spatial->angle;
            scale = spatial->scale;
            debug_color = spatial->debug_color;
            updateTransform();
        }
        else{
            cout << "Spatial NOT constructed due to the pointer not being Spatial.\n";
        }
    }
}