#include <assets.h>

void Pack::setImageAsset(const std::filesystem::__cxx11::directory_entry& file){
    std::string path = file.path().string(), name = file.path().filename().string();
    std::string clear_name = name.substr(0,name.find('.'));

    if(!std::filesystem::is_regular_file(file.status())){
        cout << "ERROR: Couldn't read file \"" << path << "\"\n"; return;
    }
    textures[clear_name] = make_shared<sf::Texture>(sf::Texture(path));
    sprites[clear_name] = make_shared<sf::Sprite>(sf::Sprite(*textures[clear_name]));
}
void Pack::setMusicAsset(const std::filesystem::__cxx11::directory_entry& file){
    std::string path = file.path().string(), name = file.path().filename().string();
    std::string clear_name = name.substr(0,name.find('.'));

    if(!std::filesystem::is_regular_file(file.status())){
        cout << "ERROR: Couldn't read file \"" << path << "\"\n"; return;
    }
    music[clear_name] = make_shared<sf::Music>(sf::Music(path));
}
void Pack::setSfxAsset(const std::filesystem::__cxx11::directory_entry& file){
    std::string path = file.path().string(), name = file.path().filename().string();
    std::string clear_name = name.substr(0,name.find('.'));

    if(!std::filesystem::is_regular_file(file.status())){
        cout << "ERROR: Couldn't read file \"" << path << "\"\n"; return;
    }
    sound_buffers[clear_name] = make_shared<sf::SoundBuffer>(sf::SoundBuffer(path));
    sfx[clear_name] = make_shared<sf::Sound>(sf::Sound(*sound_buffers[clear_name]));
}
void Pack::setFontAsset(const std::filesystem::__cxx11::directory_entry& file){
    std::string path = file.path().string(), name = file.path().filename().string();
    std::string clear_name = name.substr(0,name.find('.'));

    if(!std::filesystem::is_regular_file(file.status())){
        cout << "ERROR: Couldn't read file \"" << path << "\"\n"; return;
    }
    fonts[clear_name] = make_shared<sf::Font>(sf::Font(path));
}
weak_ptr<sf::Sprite> Pack::getSpr(const std::string key){
    return sprites[key];
}
weak_ptr<sf::Texture> Pack::getTex(const std::string key){
    return textures[key];
}

namespace assets{
    std::map<std::string,Pack> packs;

    void init(){
        loadPack("main");
    }

    void loadPack(string pack_name){
        cout << "Loading asset pack \"" << pack_name << "\"\n";
        std::filesystem::path sprites_path("assets/"+pack_name+"/sprites");
        std::filesystem::path music_path("assets/"+pack_name+"/music");
        std::filesystem::path sfx_path("assets/"+pack_name+"/sfx");
        std::filesystem::path font_path("assets/"+pack_name+"/fonts");

        packs.insert({pack_name,Pack()});

        if(std::filesystem::exists(sprites_path) && std::filesystem::is_directory(sprites_path)) {
            for(const auto& file : std::filesystem::directory_iterator(sprites_path))
                packs[pack_name].setImageAsset(file);

            for(const auto& file : std::filesystem::directory_iterator(music_path))
                packs[pack_name].setMusicAsset(file);

            for(const auto& file : std::filesystem::directory_iterator(sfx_path))
                packs[pack_name].setSfxAsset(file);

            for(const auto& file : std::filesystem::directory_iterator(font_path))
                packs[pack_name].setFontAsset(file);
        }else{
            cout << "ERROR: Couldn't find sprite pack \"" << pack_name << "\"\n";
        }
        printData();
    }
    void unloadPack(string pack_name){
        packs.erase(pack_name);
    }

    void printData(){
        for(auto &i : packs){
            std::cout << "Pack name:" << i.first << '\n';

            std::cout << "\tSprites:\n";
            for(auto &j : i.second.sprites)
                std::cout << "\t\t" << j.first << '\n';

            std::cout << "\tMusic:\n";
            for(auto &j : i.second.music)
                std::cout << "\t\t" << j.first << '\n';

            std::cout << "\tSfx:\n";
            for(auto &j : i.second.sfx)
                std::cout << "\t\t" << j.first << '\n';

            std::cout << "\tFonts:\n";
            for(auto &j : i.second.fonts)
                std::cout << "\t\t" << j.first << '\n';
        }
    }

    weak_ptr<sf::Sprite> getSp(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return packs[pack].sprites[asset];
    }
    weak_ptr<sf::Texture> getT(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return packs[pack].textures[asset];
    }
    weak_ptr<sf::Music> getM(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return packs[pack].music[asset];
    }
    weak_ptr<sf::Sound> getSf(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return packs[pack].sfx[asset];
    }
    weak_ptr<sf::Font> getF(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return packs[pack].fonts[asset];
    }
};