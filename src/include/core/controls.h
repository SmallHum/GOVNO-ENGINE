// #pragma once

#ifndef GVE_CONTROLS_H
#define GVE_CONTROLS_H

#include <config.h>

namespace controls{
    typedef short key;

    extern key A_UP,
                A_DOWN,
                A_LEFT,
                A_RIGHT,

                A_1,
                A_2,
                A_3,
                A_CNSL;

    extern key acts[];

    // Array of key states
    // 7 keys, each has 3 elements
    // Index 0 - previous held state
    // Index 1 - current held state
    // Index 2 - current toggled state
    extern bool states[8][3];

    // Function for updating controls states
    void updCtrls();

    bool kHeld(key a);
    bool kToggled(key a);
    bool kPressed(key a);

    // Gets direction vector from keys held
    // Set to UP DOWN LEFT RIGHT by default
    v2i getDHeld(key up = A_UP, key down = A_DOWN, key left = A_LEFT, key right = A_RIGHT);
    v2i getDPressed(key up = A_UP, key down = A_DOWN, key left = A_LEFT, key right = A_RIGHT);
};

#define updateControls controls::updCtrls
#define keyHeld controls::kHeld
#define keyToggled controls::kToggled
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