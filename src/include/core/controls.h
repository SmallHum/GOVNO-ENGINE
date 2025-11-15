// #pragma once

#ifndef GVE_CONTROLS_H
#define GVE_CONTROLS_H

#include <config.h>

namespace controls{
    typedef short act;

    extern act A_UP,
                A_DOWN,
                A_LEFT,
                A_RIGHT,

                A_1,
                A_2,
                A_3;

    extern act acts[256];
    extern sf::Keyboard::Key keys[7];

    // Array of act states
    // 7 keys, each has 3 elements
    // Index 0 - previous held state
    // Index 1 - current held state
    extern bool states[7][2];

    void init(
        sf::Keyboard::Key up = sf::Keyboard::Key::Up,
        sf::Keyboard::Key down = sf::Keyboard::Key::Down,
        sf::Keyboard::Key left = sf::Keyboard::Key::Left,
        sf::Keyboard::Key right = sf::Keyboard::Key::Right,

        sf::Keyboard::Key a1 = sf::Keyboard::Key::Z,
        sf::Keyboard::Key a2 = sf::Keyboard::Key::X,
        sf::Keyboard::Key a3 = sf::Keyboard::Key::C
    );

    void bind(act a, sf::Keyboard::Key key);
    void unbind(act a);

    bool kHeld(act a);
    bool kPressed(act a);

    void feedEvent(const std::optional<sf::Event> &ev);

    // Gets direction vector from keys held
    // Set to UP DOWN LEFT RIGHT by default
    v2i getDHeld(act up = A_UP, act down = A_DOWN, act left = A_LEFT, act right = A_RIGHT);
    v2i getDPressed(act up = A_UP, act down = A_DOWN, act left = A_LEFT, act right = A_RIGHT);
};

#define keyHeld controls::kHeld
#define keyPressed controls::kPressed
#define getDirHeld controls::getDHeld
#define getDirPressed controls::getDPressed

#define ACT_UP controls::A_UP
#define ACT_DOWN controls::A_DOWN
#define ACT_LEFT controls::A_LEFT
#define ACT_RIGHT controls::A_RIGHT

#define ACT_1 controls::A_1
#define ACT_2 controls::A_2
#define ACT_3 controls::A_3

#define ACT_CNSL controls::A_CNSL

#endif