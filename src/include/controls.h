#pragma once

#include <config.h>

namespace controls{
    typedef short key;

    extern key A_UP,
                A_DOWN,
                A_LEFT,
                A_RIGHT,

                A_1,
                A_2,
                A_3;

    extern key acts[];

    //array of key states
    //7 keys, each has 3 elements
    //index 0 - previous held state
    //index 1 - current held state
    //index 2 - current toggled state
    extern bool states[7][3];

    //function for updating controls states
    void updCtrls();

    bool kHeld(key a);
    bool kToggled(key a);
    bool kPressed(key a);

    //gets direction vector from keys held
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