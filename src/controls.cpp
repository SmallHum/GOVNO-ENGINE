#include <core/controls.h>

namespace controls{
    act
    A_UP = 0,
    A_DOWN = 1,
    A_LEFT = 2,
    A_RIGHT = 3,

    A_1 = 4,
    A_2 = 5,
    A_3 = 6;

    act acts[256];
    sf::Keyboard::Key keys[7];

    bool states[7][2];

    void init(
        sf::Keyboard::Key up,
        sf::Keyboard::Key down,
        sf::Keyboard::Key left,
        sf::Keyboard::Key right,

        sf::Keyboard::Key a1,
        sf::Keyboard::Key a2,
        sf::Keyboard::Key a3
    ){
        for(size_t i = 0; i < 7; i++)
            keys[i] = sf::Keyboard::Key::Unknown;

        for(size_t i = 0; i < 256; i++)
            acts[i] = -1;

        bind(A_UP, up);
        bind(A_DOWN, down);
        bind(A_LEFT, left);
        bind(A_RIGHT, right);

        bind(A_1, a1);
        bind(A_2, a2);
        bind(A_3, a3);
    }

    void bind(act act, sf::Keyboard::Key key){
        acts[(int)key] = act;
        keys[act] = key;
    }
    void unbind(act act){
        acts[(int)keys[act]] = -1;
        keys[act] = sf::Keyboard::Key::Unknown;
    }

    bool kHeld(act a){
        return states[a][1];
    }

    bool kPressed(act a){
        return !states[a][0] & states[a][1];
    }

    void feedEvent(const std::optional<sf::Event> &ev){
        if(const auto *e = ev->getIf<sf::Event::KeyPressed>()){
            int act = acts[(int)(e->code)];
            // cout << act << '\n';
            if(act != -1){
                states[act][0] = states[act][1];
                states[act][1] = 1;
            }
        }
        if(const auto *e = ev->getIf<sf::Event::KeyReleased>()){
            int act = acts[(int)(e->code)];
            // cout << act << '\n';
            if(act != -1){
                states[act][0] = states[act][1];
                states[act][1] = 0;
            }
        }
    }

    v2i getDHeld(act up, act down, act left, act right){
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

    v2i getDPressed(act up, act down, act left, act right){
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