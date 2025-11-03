// #pragma once

#ifndef GVE_ASSETS_H
#define GVE_ASSETS_H

#include <config.h>

struct Pack{
    map<string,sf::Texture> textures;
    map<string,sf::Sprite> sprites;
    map<string,sf::Music> music;
    map<string,sf::SoundBuffer> sound_buffers;
    map<string,sf::Sound> sfx;
    // map<string,sf::Font> fonts;

    // Loads independent asset map, from file
    template <typename T>
    void loadAsset(map<string, T> &assets_list, std::filesystem::path path);

    // Loads a dependent asset map, from another asset map
    template <typename T, typename Dependency>
    void loadAsset(map<string, T> &assets_list, map<string,Dependency> &d_map);

};

namespace assets{
    extern map<string,Pack> packs;

    void init();

    void loadPack(string pack_name);
    void unloadPack(string pack_name);

    sf::Sprite &getSp(const string name);
    sf::Texture &getT(const string name);
    sf::Music &getM(const string name);
    sf::Sound &getSf(const string name);
    // sf::Font &getF(const string name);

    void printData();
};

#define getSprite assets::getSp
#define getTexture assets::getT
#define getMusic assets::getM
#define getSfx assets::getSf
// #define getFont assets::getF

#endif