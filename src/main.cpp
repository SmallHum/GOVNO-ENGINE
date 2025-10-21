#include <config.h>

void exit(){
    viewport::exit();
}

void init(){
    assets::init();
    cout << "Assets init done.\n";
    viewport::init({960,720});
    cout << "Viewport init done.\n";
}

int main(){
    init();

    shared_ptr<Node> root = make_shared<Node>("root");

    sf::Clock dt_clock;
    sf::Time dt_time;
    float dt = 1.f/60.f;
    
    bool imgui_init_complete = ImGui::SFML::Init(viewport::wind);
    if(!imgui_init_complete){
        cout << "Note: couldn't init ImGUI in SFML window for some reason.\n";
    }

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

        //physics

        //render

        root->process();
        root->draw();
        root->drawDebug();

        //TEMPORARY IM TRYING TO LEARN ABOUT THIS THING
        ImGui::SFML::Update(viewport::wind, dt_time);

        ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Button("Button",ImVec2(96,48));
        ImGui::End();

        viewport::display(dt);

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}