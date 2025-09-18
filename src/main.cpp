#include <config.h>

int main(){
    shared_ptr<Node> root = make_shared<Node>("root");
    shared_ptr<Node> child_1 = make_shared<Node>("child_1");
    shared_ptr<Node> child_2 = make_shared<Node>("child_2");
    shared_ptr<Node> sasihui = make_shared<Node>("sasihui");

    child_2->addChild(sasihui);

    root->addChild(child_1);
    root->addChild(child_2);

    console::init(root);

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