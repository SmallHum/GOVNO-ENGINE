// #pragma once

#ifndef GVE_CONFIG_H
#define GVE_CONFIG_H

//sfml includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//including other stuff in panic
#include <iostream>
#include <cstdlib>
// #include <math.h>
#include <string>
// #include <vector>
#include <queue>
#include <map>
#include <set>
// #include <memory>
#include <functional>
#include <thread>
#include <fstream>
#include <filesystem>
#include <locale.h>

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
    std::pair,
    std::map,
    std::unordered_map,
    std::unordered_set,
    std::weak_ptr,
    std::shared_ptr,
    std::make_shared,
    std::function,
    std::thread,
    std::priority_queue;

const static int debug_info_z = 9999;

static sf::Color hsv(unsigned int h, unsigned int s = 100, unsigned int v = 100){
    unsigned int r, g, b;
    h = h%360;
    s = s%101;
    v = v%101;
    switch(h/60){
        case 0:
            r = 255;
            g = h*255/60;
            b = 0;
            break;
        case 1:
            r = 255 - ((h - 60)*255/60);
            g = 255;
            b = 0;
            break;
        case 2:
            r = 0;
            g = 255;
            b = (h-120)*255/60;
            break;
        case 3:
            r = 0;
            g = 255 - ((h-180)*255/60);
            b = 255;
            break;
        case 4:
            r = (h-240)*255/60;
            g = 0;
            b = 255;
            break;
        case 5:
            r = 255;
            g = 0;
            b = 255 - ((h-300)*255/60);
    }
    r += (100-s)*(255-r)/100;
    g += (100-s)*(255-g)/100;
    b += (100-s)*(255-b)/100;

    return sf::Color(r*v/100,g*v/100,b*v/100);
}

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

#endif