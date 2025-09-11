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
#include <memory>
#include <functional>
#include <thread>

#define v2f sf::Vector2f
#define v3f sf::Vector3f

#define v2i sf::Vector2i
#define v3i sf::Vector3i

#define v2u sf::Vector2u
#define v3u sf::Vector3u

using std::cin, std::cout,
    std::string,
    std::vector,
    std::weak_ptr,
    std::shared_ptr,
    std::make_shared,
    std::function,
    std::thread;

//my includes
#include <controls.h>
#include <console.h>
#include <node.h>