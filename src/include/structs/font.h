// #pragma once

#ifndef GVE_FONT_H
#define GVE_FONT_H

#include <config.h>

namespace assets{
    sf::Texture &getT(const string name);
};
#define getTexture assets::getT

// All the fonts are unfortunately monospace
// Fonts must be made from images with 16x16 character layout.
struct GVEFont : std::enable_shared_from_this<GVEFont>{

    v2i char_size;
    sf::Texture &atlas;

    shared_ptr<sf::Sprite> letters[256];

    void updateLetters(){
        for(int i = 0; i < 256; i++){
            letters[i] = make_shared<sf::Sprite>(atlas);
            v2i pos = {
                (i%16)*char_size.x,
                (i/16)*char_size.y
            };
            letters[i]->setTextureRect(sf::IntRect(pos, char_size));
            // letters[i]->move(-v2f(pos));
        }
    }

    GVEFont(sf::Texture &atlas):
        atlas(atlas){

        char_size.x = atlas.getSize().x/16;
        char_size.y = atlas.getSize().y/16;

        updateLetters();
    }

    sf::Sprite *getGlyph(unsigned char a){
        // cout << letters[a] << '\n';
        return letters[a].get();
    }

    GVEFont operator = (const GVEFont &other){
        atlas = other.atlas;
        char_size = other.char_size;
        updateLetters();

        return *this;
    }
};

namespace assets{
    GVEFont &getF(const string name);
    string getFontName(GVEFont &font);
};
#define getFont assets::getF

#endif