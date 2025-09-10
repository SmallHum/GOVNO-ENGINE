#include <controls.h>

namespace controls{
    key A_UP = 0,
        A_DOWN = 1,
        A_LEFT = 2,
        A_RIGHT = 3,

        A_1 = 4,
        A_2 = 5,
        A_3 = 6,
        A_CNSL = 7;

    key acts[] = {VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,'Z','X','C',VK_F9};

    bool states[8][3];

    void updCtrls(){
            for(int i = 0; i < 8; i++){
                states[i][0] = states[i][1];
                states[i][1] = GetKeyState(acts[i]) & 0x8000;
                states[i][2] = GetKeyState(acts[i]) & 1;
            }
        }

    bool kHeld(key a){
        return states[a][1];
    }

    bool kToggled(key a){
        return states[a][2];
    }

    bool kPressed(key a){
        return !states[a][0] & states[a][1];
    }

    v2i getDHeld(key up, key down, key left, key right){
        v2i dir;

        if(keyHeld(up)){
            dir.y--;
        }
        if(keyHeld(down)){
            dir.y++;
        }
        if(keyHeld(left)){
            dir.x--;
        }
        if(keyHeld(right)){
            dir.x++;
        }
        
        return dir;
    }

    v2i getDPressed(key up, key down, key left, key right){
        v2i dir;

        if(keyPressed(up)){
            dir.y--;
        }
        if(keyPressed(down)){
            dir.y++;
        }
        if(keyPressed(left)){
            dir.x--;
        }
        if(keyPressed(right)){
            dir.x++;
        }
        
        return dir;
    }
};