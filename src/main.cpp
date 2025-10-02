#include <config.h>

void exitEverything(){
    viewport::exit();
    console::destroy();
}

int main(){
    shared_ptr<Node> root = make_shared<Node>("root");
    shared_ptr<Node> spatial_1 = make_shared<Spatial>(
        "spatial_1", 
        v2f(128.f,128.f),
        0,
        v2f(1.f, 1.f),
        sf::Color::Green
    );
    shared_ptr<Node> child_2 = make_shared<Node>("child_2");
    shared_ptr<Node> sasihui = make_shared<Spatial>(
        "sasihui", 
        v2f(128.f,-64.f),
        0,
        v2f(1.0f, 1.f),
        sf::Color::Red
    );

    spatial_1->addChild(sasihui);

    root->addChild(spatial_1);
    root->addChild(child_2);

    console::init(root);

    sf::Text text(viewport::default_font,"If you're reading this, \ni, Max Aplex, am already dead.\nLife in Hiroshima was great,\nuntil 1945 of corse. I should've\ndied, but instead, i've turned red.\nNow i don't have any\nother things to do, but to \nclose every door i see.");
    text.setFillColor({255,255,255});

    text.move({128,128});

    sf::Clock dt_clock;
    float dt = 1.f/60.f;

    viewport::init({960,720});

    viewport::wind.setFramerateLimit(60);

    //main loop
    while(viewport::wind.isOpen()){
        dt_clock.start();
        //update events
        while(const std::optional ev = viewport::wind.pollEvent()){
            if(ev->is<sf::Event::Closed>()){
                exitEverything();
            }
        }
        updateControls();

        if(keyPressed(ACT_CNSL))
            console::flip();

        //physics
        text.move(64.f*v2f(getDirHeld())*dt);

        //render

        viewport::bg_color = (console::is_open) ? sf::Color(43,67,175) : sf::Color(0,0,0);

        dynamic_cast<Spatial*>(spatial_1.get())->angle += 2*16.f * dt;
        dynamic_cast<Spatial*>(sasihui.get())->angle -= 2*12.f * dt;

        root->process();
        root->draw();
        root->drawDebug();

        viewport::display(dt);

        dt = dt_clock.reset().asSeconds();
    }
    exitEverything();
}