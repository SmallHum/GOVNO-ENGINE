#include <core/controls.h>
#include <core/viewport.h>
#include <structs/spatial.h>
#include <fstream_opers.h>

void init(){
    assets::init();
    cout << "Assets init done.\n";
    viewport::init({960,720});
    cout << "Viewport init done.\n";
    factory::init();
    cout << "Node loader init done.\n";

    viewport::showSpatialOrigin();
    viewport::showSpatialName();
}

void exit(){
    viewport::exit();
}

int main(){
    init();

    // Trying to write it;
    shared_ptr<Node> root = make_shared<Node>("root");
    shared_ptr<Node> child = make_shared<Node>("child");

    root->addChild(child);
    root->addChild(make_shared<Node>("idiot1"));
    child->addChild(make_shared<Node>("idiot2"));

    root->writeToFile("root.bin");

    root->printTree();

    // Trying to read it
    shared_ptr<Node> root2 = constructFromFile("root.bin");

    root2->printTree();

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