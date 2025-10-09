#include <config.h>

void exit(){
    viewport::exit();
    console::destroy();
}

void init(){
    console::init();
    cout << "Console init done.\n";
    assets::init();
    cout << "Assets init done.\n";
    viewport::init({960,720});
    cout << "Viewport init done.\n";
}

int main(){
    init();

    shared_ptr<Node> root = make_shared<Node>("root");
    console::setRoot(root);

    sf::Clock dt_clock;
    float dt = 1.f/60.f;

    //main loop
    while(viewport::wind.isOpen()){
        dt_clock.start();
        //update events
        while(const std::optional ev = viewport::wind.pollEvent()){
            if(ev->is<sf::Event::Closed>()){
                exit();
            }
        }
        updateControls();

        if(keyPressed(ACT_CNSL))
            console::flip();

        //physics

        //render

        viewport::bg_color = (console::is_open) ? sf::Color(43,67,175) : sf::Color(0,0,0);

        root->process();
        root->draw();
        root->drawDebug();

        viewport::display(dt);

        dt = dt_clock.reset().asSeconds();
    }
    exit();
}