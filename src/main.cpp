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
    sf::Time dt_time;
    float dt = 1.f/60.f;

    //main loop
    while(viewport::wind.isOpen()){
        //update events
        while(const std::optional ev = viewport::wind.pollEvent()){
            ImGui::SFML::ProcessEvent(viewport::wind,*ev);
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

        ImGui::SFML::Update(viewport::wind, dt_time);

        ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoMove);
        ImGui::Button("Button",ImVec2(96,48));
        ImGui::End();

        viewport::display(dt);

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}