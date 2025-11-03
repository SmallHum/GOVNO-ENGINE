#include <core/assets.h>

// Loads independent asset map, from file
template <typename T>
void Pack::loadAsset(
        map<string, T> &assets_list, 
        std::filesystem::path path
){
    if(std::filesystem::exists(path) && std::filesystem::is_directory(path)) {

            for(const auto& file : std::filesystem::directory_iterator(path)){

                // Getting a clear name and path string
                std::string str_path = file.path().string(),
                name = file.path().filename().string();
                std::string clear_name = name.substr(0,name.find('.'));

                // Checking if file is even readable.
                // Skipping if not. It will crash anyways.
                if(!std::filesystem::is_regular_file(file.status())){
                    cout << "ERROR: Couldn't read file \"" << str_path << "\"\n"; continue;
                }
                // assets_list.insert(
                //     pair<const string, T>(clear_name, T(str_path))
                // );
                assets_list.insert_or_assign(clear_name, T(str_path));
            }
    }
    else{
        cout << "ERROR: Couldn't find folder \"" << path.string() << "\"\n";
    }
}

// Loads a dependent asset map, from another asset map
template <typename T, typename Dependency>
void Pack::loadAsset(
        map<string, T> &assets_list, 
        map<string,Dependency> &d_map
){
    for(auto &i : d_map){
        // assets_list.insert(
        //     pair<const string, Dependency>(i.first, T(i.second))
        // );
        assets_list.insert_or_assign(i.first, T(i.second));
    }
}

template<typename Key, typename Val>
Val &find(map<Key, Val> &map, Key key){
    auto result = map.find(key);
    if(result == map.end()){
        cout << "Value by key " << key << "not found.\n";
        throw std::exception();
    }
    return result->second;
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
        // std::filesystem::path font_path("assets/"+pack_name+"/fonts");

        packs.insert({pack_name,Pack()});

        auto &pack = packs[pack_name];

        pack.loadAsset(pack.textures,sprites_path);
        pack.loadAsset(pack.sprites,pack.textures);

        pack.loadAsset(pack.music, music_path);

        pack.loadAsset(pack.sound_buffers, sfx_path);
        pack.loadAsset(pack.sfx, pack.sound_buffers);

        // pack.loadAsset<sf::Font>(pack.fonts,font_path);
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

            // cout << "  Fonts:\n";
            // for(auto &j : i.second.fonts)
            //     cout << "    " << j.first << '\n';
        }
        cout << '\n';
    }

    sf::Sprite &getSp(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return find(packs[pack].sprites, asset);
    }
    sf::Texture &getT(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return find(packs[pack].textures, asset);
    }
    sf::Music &getM(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return find(packs[pack].music, asset);
    }
    sf::Sound &getSf(const string name){
        size_t colon_index = name.find_first_of(':');
        string pack = name.substr(0,colon_index),
                asset = name.substr(colon_index+1,name.length()-colon_index-1);

        return find(packs[pack].sfx, asset);
    }

    // i think we dont need it anymore
    // sf::Font &getF(const string name){
    //     size_t colon_index = name.find_first_of(':');
    //     string pack = name.substr(0,colon_index),
    //             asset = name.substr(colon_index+1,name.length()-colon_index-1);

    //     return find(packs[pack].fonts, asset);
    // }
};