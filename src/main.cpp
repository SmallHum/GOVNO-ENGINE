#include <config.h>
    
shared_ptr<Node> root = make_shared<Node>("root");
shared_ptr<Node> child_1 = make_shared<Node>("child_1");
shared_ptr<Node> child_2 = make_shared<Node>("child_2");
shared_ptr<Node> sasihui = make_shared<Node>("sasihui");

namespace console{
    void inputHandle(){
        string command;
        while(!is_destroying){
            root->printTree();
            cout << "> ";
            cin >> command;
            if(command == "help"){
                help();
            }
            else if(command == "find-node"){
                findNode();
            }
            else if(command == "make-node"){
                makeNode();
            }
            else{
                cout << "ERROR: wrong command. Type 'help' to list commands.\n\n";
            }
        }
    }
    void findNode(){
        string path;
        cin >> path;
        shared_ptr<Node> result = root->find(path);
        if(result)
            cout << "Node found successfully! (" << result->name << ")\n\n";
        else cout << "ERROR: invalid path.\n\n";
    }
    void makeNode(){
        string _class, parent_path, name;

        cin >> _class >> parent_path >> name;

        shared_ptr<Node> node, parent = root->find(parent_path);

        if(!parent) {
            cout << "ERROR: invalid parent path.\n\n"; return;
        }

        if(_class == "Node"){
            node.reset(new Node(name));
            parent->addChild(node);
        }
        else {
            cout << "ERROR: invalid class.\n\n"; return;
        }

        cout << "Node " << name << " created successfully!\n\n";
    }
}

int main(){
    child_2->addChild(sasihui);

    root->addChild(child_1);
    root->addChild(child_2);

    console::init();

    sf::RenderWindow wind(sf::VideoMode({960,720}),"Test window");

    sf::Font default_font("assets/font/cour.ttf");

    sf::Text text(default_font,"If you're reading this, \ni, Max Aplex, am already dead.\nLife in Hiroshima was great,\nuntil 1945 of corse. I should've\ndied, but instead, i've turned red.\nNow i don't have any\nother things to do, but to \nclose every door i see.");
    text.setFillColor({255,255,255});

    text.move({128,128});

    sf::Clock dt_clock;
    float dt = 1.f/60.f;

    wind.setFramerateLimit(60);

    //main loop
    while(1){
        dt_clock.start();
        //update events
        while(const std::optional ev = wind.pollEvent()){
            if(ev->is<sf::Event::Closed>()){
                wind.close();
                console::destroy();
                std::exit(0);
            }
        }
        updateControls();

        if(keyPressed(ACT_CNSL))
            console::flip();

        //physics
        text.move(64.f*v2f(getDirHeld())*dt);

        //render
        wind.clear((console::is_open) ? sf::Color(43,67,175) : sf::Color(0,0,0));
        wind.draw(text);

        wind.display();

        dt = dt_clock.reset().asSeconds();
    }

}