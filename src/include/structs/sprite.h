// #pragma once

#ifndef GVE_SPRITE_H
#define GVE_SPRITE_H

#include <structs/spatial.h>

//тоже спиздить из старо
//короче всё спиздить из старого проекта и сделать лучше
struct Sprite : Spatial{
    struct Animation{

    };

    shared_ptr<Animation> anim;
};

#endif