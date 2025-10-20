#include <core/assets.h>

// Loads independent asset map, from file
template <typename T>
void Pack::loadAsset(
        map<string, shared_ptr<T>> &assets_list, 
        std::filesystem::path path
){
    if(std::filesystem::exists(path) && std::filesystem::is_directory(path)) {

            for(const auto& file : std::filesystem::directory_iterator(path)){

                // Getting a clear name and path string
                std::string path = file.path().string(),
                name = file.path().filename().string();
                std::string clear_name = name.substr(0,name.find('.'));

                // Checking if file is even readable.
                // Skipping if not. It will crash anyways.
                if(!std::filesystem::is_regular_file(file.status())){
                    cout << "ERROR: Couldn't read file \"" << path << "\"\n"; continue;
                }
                assets_list[clear_name] = make_shared<T>(T(path));
            }
    }
    else{
        cout << "ERROR: Couldn't find folder \"" << path.string << "\"\n";
    }
}

// Loads a dependent asset map, from another asset map
template <typename T, typename Dependency>
void Pack::loadAsset(
        map<string, shared_ptr<T>> &assets_list, 
        map<string,shared_ptr<Dependency>> &d_map
){
    for(auto &i : d_map){
        assets_list[i.first] = make_shared<T>(T(*i.second));
    }
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
        cout << "Loading asset pack \"" << pack_name << "\"...\n\n";
        std::filesystem::path sprites_path("assets/"+pack_name+"/sprites");
        std::filesystem::path music_path("assets/"+pack_name+"/music");
        std::filesystem::path sfx_path("assets/"+pack_name+"/sfx");
        std::filesystem::path font_path("assets/"+pack_name+"/fonts");

        packs.insert({pack_name,Pack()});

        auto &pack = packs[pack_name];

        pack.loadAsset<sf::Texture>(pack.textures,sprites_path);
        pack.loadAsset<sf::Sprite,sf::Texture>(pack.sprites,pack.textures);

        pack.loadAsset<sf::Music>(pack.music, music_path);

        pack.loadAsset<sf::SoundBuffer>(pack.sound_buffers, sfx_path);
        pack.loadAsset<sf::Sound,sf::SoundBuffer>(pack.sfx, pack.sound_buffers);
        printData();
    }
    void unloadPack(string pack_name){
        packs.erase(pack_name);
    }

    void printData(){
        for(auto &i : packs){
            cout << "Pack name:" << i.first << '\n';

            cout << "  Sprites:\n";
            for(auto &j : i.second.sprites)
                cout << "    " << j.first << '\n';

            cout << "  Music:\n";
            for(auto &j : i.second.music)
                cout << "    " << j.first << '\n';

            cout << "  Sfx:\n";
            for(auto &j : i.second.sfx)
                cout << "    " << j.first << '\n';

            cout << "  Fonts:\n";
            for(auto &j : i.second.fonts)
                cout << "    " << j.first << '\n';
        }
        cout << '\n';
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