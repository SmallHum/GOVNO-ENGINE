#pragma once

//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//including other stuff in panic
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include <thread>
#include <fstream>
#include <filesystem>

#define v2f sf::Vector2f
#define v3f sf::Vector3f

#define v2i sf::Vector2i
#define v3i sf::Vector3i

#define v2u sf::Vector2u
#define v3u sf::Vector3u

#define mat3 sf::Transform

using std::cin, std::cout,
    std::fstream,
    std::string,
    std::vector,
    std::map,
    std::unordered_map,
    std::set,
    std::weak_ptr,
    std::shared_ptr,
    std::make_shared,
    std::function,
    std::thread,
    std::priority_queue;

const static int debug_info_z = 9999;

static inline string prettyBool(bool a){
    return a ? "True" : "False";
}

template <typename T>
static std::ostream& operator << (std::ostream& s, sf::Vector2<T> v){
    s << v.x << ' ' << v.y;
    return s;
}

static std::ostream& operator << (std::ostream& s, mat3 v){
    for(int i = 0; i < 16; i++)
        s << v.getMatrix()[i] << ' ';
    return s;
}