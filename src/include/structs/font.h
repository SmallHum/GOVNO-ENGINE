// #pragma once

#ifndef GVE_FONT_H
#define GVE_FONT_H

#include <config.h>

// All the fonts are unfortunately monospace
// Fonts must be made from images with 16x16 character layout.
struct GVEFont : std::enable_shared_from_this<GVEFont>{

    v2i char_size;
    sf::Texture &atlas;

    sf::Sprite *letters[256] = {nullptr};

    GVEFont(sf::Texture &atlas): atlas(atlas){
        char_size.x = atlas.getSize().x/16;
        char_size.y = atlas.getSize().y/16;

        for(int i = 0; i < 256; i++){
            letters[i] = new sf::Sprite(atlas);
            v2i pos = {
                (i%16)*char_size.x,
                (i/16)*char_size.y
            };
            letters[i]->setTextureRect(sf::IntRect(pos, char_size));
            // letters[i]->move(-v2f(pos));
        }
    }

    sf::Sprite *getGlyph(unsigned char a){
        return letters[a];
    }

    ~GVEFont(){
        for(auto i : letters)
            delete i;
    }
};

#endif