#include <SFML\Graphics.hpp>

#define v2f sf::Vector2f

int main(){
    sf::RenderWindow wind(sf::VideoMode({960,720}),"Test window");

    sf::Font default_font("assets/font/cour.ttf");

    sf::Text text(default_font,"If you're reading this, \ni, Max Aplex, am already dead.\nLife in Hiroshima was great,\nuntil 1945 of corse. I should've\ndied, but instead, i've turned red.\nNow i don't have any\nother things to do,but to \nclose every door i see.");
    text.setFillColor({255,255,255});

    text.move({128,128});

    wind.clear({43,67,175});
    wind.draw(text);

    wind.display();

    while(1){
        while(const std::optional ev = wind.pollEvent()){
            if(ev->is<sf::Event::Closed>())
                wind.close();
        }
    }

}