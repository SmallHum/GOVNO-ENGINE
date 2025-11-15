#include <structs/aabb.h>
#include <core/viewport.h>
#include <struct_loader.h>

AABB::AABB(): Spatial(){
    name = "AABB";
    top = -32.f;
    bottom = 32.f;
    left = -32.f;
    right = 32.f;
}

AABB::AABB(string name,
    v2f pos, float angle, v2f scale,
    float top, float bottom, float left, float right,
    sf::Color debug_color): Spatial(
        name,
        pos, angle, scale,
        debug_color
    ), top(top),
        bottom(bottom),
        left(left),
        right(right){

    }

bool AABB::intersectPoint(shared_ptr<Node> spatial){
    auto s = dynamic_cast<Spatial*>(spatial.get());
    if(!s){cout << "WARNING: invalid Spatial pointer parsed to AABB::intersectPoint"; return 0;}
    return intersectPoint(s->getGlobalPos());
}
bool AABB::intersectPoint(v2f pos){
    v2f global_pos = getGlobalPos();
    float global_top = global_pos.y + top;
    float global_bottom = global_pos.y + bottom;
    float global_left = global_pos.x + left;
    float global_right = global_pos.x + right;

    return global_left <= pos.x && pos.x <= global_right &&
            global_top <= pos.y && pos.y <= global_bottom;
}

bool AABB::intersectAABB(shared_ptr<Node> other){
    auto o = dynamic_cast<AABB*>(other.get());
    if(!o){cout << "WARNING: invalid AABB pointer parsed to AABB::intersectAABB"; return 0;}
    v2f global_pos = getGlobalPos(),
        other_global_pos = o->getGlobalPos();

    float global_top = global_pos.y + top;
    float global_bottom = global_pos.y + bottom;
    float global_left = global_pos.x + left;
    float global_right = global_pos.x + right;
    
    float other_global_top = other_global_pos.y + o->top;
    float other_global_bottom = other_global_pos.y + o->bottom;
    float other_global_left = other_global_pos.x + o->left;
    float other_global_right = other_global_pos.x + o->right;

    cout << "trying btw...\n";

    return global_top <= other_global_bottom && global_bottom >= other_global_top &&
            global_left <= other_global_right && global_right >= other_global_left;
}

StructId AABB::getStructId(){
    return StructId::AABB;
}

void AABB::drawDebug(){
    Spatial::drawDebug();
    if(debug::show_aabb_bounds){
        viewport::drawOver(
            debug::aabb_bounds(top, bottom, left, right, debug_color), 1, 0,
            mat3().translate(getGlobalPos())
        );
    }
}

void AABB::process(){
    Spatial::process();
}

void AABB::reader(fstream &stream){
    Spatial::reader(stream);

    top = fstreamRead<float>(stream);
    bottom = fstreamRead<float>(stream);
    left = fstreamRead<float>(stream);
    right = fstreamRead<float>(stream);
}
void AABB::writer(fstream &stream){
    Spatial::writer(stream);

    fstreamWrite<float>(stream, top);
    fstreamWrite<float>(stream, bottom);
    fstreamWrite<float>(stream, left);
    fstreamWrite<float>(stream, right);
}

void AABB::copy(weak_ptr<Node> node){
    Spatial::copy(node);
    if(auto n = node.lock()){
        auto aabb = dynamic_cast<AABB*>(n.get());
        if(aabb){
            top = aabb->top;
            bottom = aabb->bottom;
            left = aabb->left;
            right = aabb->right;
        }
        else{
            cout << "AABB NOT constructed due to the pointer not being AABB.\n";
        }
    }
}