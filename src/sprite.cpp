#include <structs/sprite.h>

Sprite::Sprite(): 
    Spatial()
{
    name = "Sprite";
    anim.push_back(Animation());
    current_animation_index = z = 0;
}

Sprite::Sprite(
    string name,
    v2f pos, 
    float angle, v2f scale,
    Animation anim,
    int z,
    sf::Color debug_color
): 
    Spatial(
        name,
        pos,
        angle,
        scale,
        debug_color
        ),
    z(z)
{
    this->anim.push_back(anim);
    current_animation_index = 0;
}

StructId Sprite::getStructId(){
    return StructId::Sprite;
}