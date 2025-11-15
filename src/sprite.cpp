#include <structs/sprite.h>

Sprite::Sprite(): 
    Spatial()
{
    name = "Sprite";
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
    anim(anim),
    z(z)
{

}

StructId Sprite::getStructId(){
    return StructId::Sprite;
}