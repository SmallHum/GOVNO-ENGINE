#pragma once

//sfml includes
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

//including other stuff in panic
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <functional>
#include <thread>

#define v2f sf::Vector2f
#define v3f sf::Vector3f

#define v2i sf::Vector2i
#define v3i sf::Vector3i

#define v2u sf::Vector2u
#define v3u sf::Vector3u

#define mat3 sf::Transform

using std::cin, std::cout,
    std::string,
    std::vector,
    std::map,
    std::weak_ptr,
    std::shared_ptr,
    std::make_shared,
    std::function,
    std::thread,
    std::priority_queue;

//my includes
#include <node.h>
#include <controls.h>
#include <console.h>

static inline string prettyBool(bool a){
    return a ? "True" : "False";
}