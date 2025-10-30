#pragma once

#include <config.h>

// All the fonts are unfortunately monospace
// Fonts must be made from images with 16x16 character layout.
struct Font{

    v2i char_size;
    sf::Texture &atlas;

    sf::Sprite *letters[256] = {nullptr};

    Font(sf::Texture &atlas): atlas(atlas){
        char_size.x = atlas.getSize().x/16;
        char_size.y = atlas.getSize().y/16;

        for(int i = 0; i < 256; i++){
            sf::Sprite *result = new sf::Sprite(atlas);
            v2i pos = {
                i%16,
                i/16
            };
            result->setTextureRect(sf::IntRect(pos, char_size));
            result->move(v2f(-pos));
            letters[i] = result;
        }
    }

    sf::Sprite *getGlyph(unsigned char a){
        return letters[a];
    }

    ~Font(){
        for(auto i : letters)
            delete i;
    }
};