#include <core/controls.h>
#include <core/assets.h>

#include <struct_loader.h>

void init(){
    assets::init();
    cout << "Assets init done.\n";
    viewport::init({960,720});
    cout << "Viewport init done.\n";
    debug::init();
    cout << "Debug init done.\n";
    factory::init();
    cout << "Node loader init done.\n";

    debug::showAll();
}

void exit(){
    viewport::exit();
}

int main(){
    init();

    shared_ptr<Node> root = constructFromFile("saved.ntr");

    root->printTree();

    sf::Clock dt_clock;
    sf::Time dt_time;
    float dt = 1.f/60.f;

    //main loop
    while(viewport::wind.isOpen()){
        //update events
        while(const std::optional ev = viewport::wind.pollEvent()){
            if(ev->is<sf::Event::Closed>()){
                exit();
            }
        }
        updateControls();

        //physics

        //render

        root->process();
        root->draw();
        root->drawDebug();

        viewport::display(dt);
        viewport::wind.display();

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}