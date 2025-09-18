#include <spatial.h>

inline void Spatial::updateLocalTransform(){
    local_transform = sf::Transform().scale(scale).rotate(sf::degrees(angle)).translate(pos);
}

inline void Spatial::updateTransform(){
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
    return v2f(global_transform.getMatrix()[2],global_transform.getMatrix()[5]);
}