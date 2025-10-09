#pragma once

#include <config.h>

struct Pack{
    map<string,shared_ptr<sf::Texture>> textures;
    map<string,shared_ptr<sf::Sprite>> sprites;
    map<string,shared_ptr<sf::Music>> music;
    map<string,shared_ptr<sf::SoundBuffer>> sound_buffers;
    map<string,shared_ptr<sf::Sound>> sfx;
    map<string,shared_ptr<sf::Font>> fonts;

    void setImageAsset(const std::filesystem::__cxx11::directory_entry& file);
    void setMusicAsset(const std::filesystem::__cxx11::directory_entry& file);
    void setSfxAsset(const std::filesystem::__cxx11::directory_entry& file);
    void setFontAsset(const std::filesystem::__cxx11::directory_entry& file);

    weak_ptr<sf::Sprite> getSpr(const string name);
    weak_ptr<sf::Texture> getTex(const string name);
    weak_ptr<sf::Music> getMusic(const string name);
    weak_ptr<sf::Sound> getSfx(const string name);
    weak_ptr<sf::Font> getFont(const string name);

};

namespace assets{
    extern map<string,Pack> packs;

    void init();

    void loadPack(string pack_name);
    void unloadPack(string pack_name);

    weak_ptr<sf::Sprite> getSp(const string name);
    weak_ptr<sf::Texture> getT(const string name);
    weak_ptr<sf::Music> getM(const string name);
    weak_ptr<sf::Sound> getSf(const string name);
    weak_ptr<sf::Font> getF(const string name);

    void printData();
};

#define getSprite assets::getSp
#define getTexture assets::getT
#define getMusic assets::getM
#define getSfx assets::getSf
#define getFont assets::getF