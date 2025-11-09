#define EDITOR

#include <core/controls.h>
#include <core/assets.h>
#include <core/viewport.h>
#include <struct_loader.h>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui-SFML.h>

#define CTRL_DOWN (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || \
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))

#define SHIFT_DOWN (ImGui::IsKeyDown(ImGuiKey_LeftShift) || \
        ImGui::IsKeyDown(ImGuiKey_RightShift))

#define PRESSED(K) ImGui::IsKeyPressed(K)
#define DOWN(K) ImGui::IsKeyDown(K)

#define SHIFT(K) (SHIFT_DOWN && PRESSED(K))
#define CTRL(K) (CTRL_DOWN && PRESSED(K))

#define SHIFT_AND(COND) (SHIFT_DOWN && (COND))
#define CTRL_AND(COND) (CTRL_DOWN && (COND))

namespace node_gen{

    shared_ptr<Node> randomType(){
        StructId id = (StructId)(rand()%factory::creators.size());
        shared_ptr<Node> result = factory::creators[id]();

        result->name += ' ';
        
        size_t name_size = rand()%5 + 2;
        for(size_t i = 0; i < name_size; i++){
            char rand_cahr = (rand()%2) ? (rand() % ('z'-'a'+1) + 'a') : (rand() % ('Z'-'A'+1) + 'A');
            result->name += rand_cahr;
        }

        if(auto s = dynamic_cast<Spatial*>(result.get())){
            s->pos = {rand()%100 - 50, rand()%100 - 50};
            s->angle = rand()%360;
            s->scale = {((rand()%35)+15) / 25.f, ((rand()%35)+15)/25.f};
        }

        return result;
    }

    void genTree(weak_ptr<Node> root, int max_depth, int depth = 0){
        if(depth == max_depth)
            return;
        int kids = 1+rand()%3;
        for(int i = 0; i < kids; i++){
            shared_ptr<Node> n = randomType();
            root.lock()->addChild(n);
            genTree(n,max_depth,depth+1);
        }
    }
};

namespace editor{

    shared_ptr<Node> node_root;
    weak_ptr<Node> selection;

    void init(){
        node_root = make_shared<Node>("root");
        node_gen::genTree(node_root, 3);

        // other = make_shared<AABB>();
        // node_root->addChild(other);

        node_root->printTree();
    }

    void open(){
        if(!node_root){
            cout << "HOW DID YOU DELETE NODE ROOT LMAO\n"; return;
        }
        node_root = constructFromFile("saved.ntr");
    }

    void save(){
        if(!node_root){
            cout << "HOW DID YOU DELETE NODE ROOT LMAO\n"; return;
        }
        node_root->writeToFile("saved.ntr");
    }

    void editFieldBool(string label, bool &v){
        ImGui::Checkbox(label.c_str(), &v);
    }

    void editFieldFloat(string label, float &v){
        float buffer = v;

        if(!ImGui::InputFloat(label.c_str(), &buffer))
            return;

        if(!PRESSED(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v = buffer;
    }

    void editFieldV2F(string label, v2f &v){
        float buffer[2] = {v.x, v.y};

        if(!ImGui::InputFloat2(label.c_str(), buffer))
            return;
        if(!PRESSED(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v.x = buffer[0];
        v.y = buffer[1];
    }

    void editFieldString(string label, string &v, bool multiline = 0){
        string buffer = v;

        if(multiline){
            if(!ImGui::InputTextMultiline(label.c_str(), &buffer))
                return;
        }
        else{
            if(!ImGui::InputText(label.c_str(), &buffer))
                return;
        }
        if(!PRESSED(ImGuiKey_Enter))
                return;
        

        cout << "    applying " << label << '\n';
        v.resize(strlen(buffer.c_str()));
        v = buffer;
    }

    void LabelEditMenu(Label *node){
        ImGui::SeparatorText("Label");

        editFieldString("Text", node->text, true);
    }

    void AABBEditMenu(AABB *node){
        ImGui::SeparatorText("AABB");

        editFieldFloat("Top", node->top);
        editFieldFloat("Bottom", node->bottom);
        editFieldFloat("Left", node->left);
        editFieldFloat("Right", node->right);
    }

    void spatialEditMenu(Spatial *node){
        ImGui::SeparatorText("Spatial");

        editFieldV2F("Position", node->pos);
        editFieldFloat("Angle", node->angle);
        editFieldV2F("Scale", node->scale);

        auto aabb = dynamic_cast<AABB*>(node);
        if(aabb)AABBEditMenu(aabb);

        auto label = dynamic_cast<Label*>(node);
        if(label)LabelEditMenu(label);
    }

    void nodeEditMenu(Node *node){
        ImGui::SeparatorText("Node");

        editFieldString("Name", node->name);
        editFieldBool("Visible", node->visible);

        auto s = dynamic_cast<Spatial*>(node);
        if(s)spatialEditMenu(s);
    }

    int id_fix = 0;
    int sel_margin = 20;

    bool any_editor_window_focused = 0,
        controlling_camera = 0;

    v2f cam_pos = {-480.f, -360.f};

    float tree_view_width = 240,
        actions_width = 160,
        bar_height_button_size = 26,
        edit_width = 240,
        edit_pos_x;

    float wheel_delta_x, wheel_delta_y;

    shared_ptr<Node> getHovered(shared_ptr<Node> root, weak_ptr<Node> avoid){
        if(root == avoid.lock())return shared_ptr<Node>();

        Spatial *sp = dynamic_cast<Spatial *>(root.get());

        if(sp){
            auto m = ImGui::GetMousePos();
            if((v2f(m.x,m.y) - sp->getGlobalPos() + cam_pos).length() <= sel_margin){
                return root;
            }
        }

        for(auto &i : root->children){
            auto result = getHovered(i, avoid);
            if(result)return result;
        }

        return shared_ptr<Node>();
    }

    void process(float &dt){

        // if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
        //     selection = getHovered(node_root, selection);
        //     cout << selection.lock()->name << '\n';
        // }

        id_fix = 0;

        wheel_delta_x = wheel_delta_y = 0.f;

        // Menu bar.
        // TODO: 
        // - Copy, cut, paste.
        // - Folder view on open and save.
        // - Discartion alert on opening.
        if(ImGui::BeginMainMenuBar()){
            if(ImGui::BeginMenu("File")){
                if(ImGui::MenuItem("Open", "Ctrl+O"))open();
                if(ImGui::MenuItem("Save", "Ctrl+S"))save();
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Edit")){
                if(ImGui::MenuItem("New Node", "Ctrl+N"))ImGui::OpenPopup("Make Node");
                if(ImGui::MenuItem("Delete Node", "Del")){}
                ImGui::Separator();
                if(ImGui::MenuItem("Copy", "Ctrl+C")){}
                if(ImGui::MenuItem("Paste", "Ctrl+V")){}
                ImGui::Separator();
                if(ImGui::MenuItem("Move Camera", "Shift+F"))controlling_camera = 1;
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Keyboard actions.
        if(PRESSED(ImGuiKey_Delete)){}
        if(CTRL(ImGuiKey_N))
            ImGui::OpenPopup("Make Node");
        if(CTRL(ImGuiKey_C)){}
        if(CTRL(ImGuiKey_S))
            save();
        if(CTRL(ImGuiKey_V)){}
        if(CTRL(ImGuiKey_O))
            open();
        if(SHIFT(ImGuiKey_F))
            controlling_camera = 1;
        if(CTRL(ImGuiKey_Z)){}
        if(CTRL(ImGuiKey_Y)){}

        float height = viewport::wind.getSize().y-bar_height_button_size;

        // Tree view window.
        ImGui::SetNextWindowPos({0,bar_height_button_size});
        ImGui::SetNextWindowSize({tree_view_width,height});
        ImGui::SetNextWindowSizeConstraints(
            {240,height},
            {FLT_MAX,height}
        );
        ImGui::Begin("Tree view", nullptr, 
            ImGuiWindowFlags_NoMove 
            | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoScrollWithMouse);

        tree_view_width = ImGui::GetWindowSize().x;

        // ImGui::Columns(2);
        
        // tree_root->drawCol1(active_selection, multiple_selection);
        // ImGui::NextColumn();
        
        // tree_root->drawCol2();

        ImGui::End();

        // Node editing window.
        // Kinda stolen from Godot.
        edit_pos_x = viewport::wind.getSize().x - edit_width;
        ImGui::SetNextWindowPos({edit_pos_x,bar_height_button_size});
        ImGui::SetNextWindowSize({edit_width,height});
        ImGui::SetNextWindowSizeConstraints(
            {240,height},
            {FLT_MAX,height}
        );
        ImGui::Begin("Edit", nullptr, 
            ImGuiWindowFlags_NoMove 
            | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoScrollWithMouse);


        if(auto s = selection.lock())
            nodeEditMenu(s.get());

        ImGui::End();

        // Make node popup.
        // Need to upgrade it.
        if(ImGui::BeginPopup("Make Node")){

            for(auto& i : factory::creators){
                if(ImGui::Button(factory::names[(size_t)i.first].c_str(),{64,26})){
                    // newNode(i.second);
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
        
        any_editor_window_focused = ImGui::IsWindowFocused(
            ImGuiFocusedFlags_AnyWindow | 
            ImGuiFocusedFlags_ChildWindows
        ) || ImGui::IsAnyItemFocused();

        if(any_editor_window_focused)return;

        cam_pos.x += wheel_delta_x * 5.f;
        cam_pos.y += wheel_delta_y * 5.f;

        if(auto s = selection.lock()){
            auto p = dynamic_cast<Spatial*>(s.get());
            if(p){
                p->pos += 150.f * dt * v2f(getDirHeld());
                if(PRESSED(ImGuiKey_Q))p->angle += 50.f * dt;
                if(PRESSED(ImGuiKey_W))p->angle -= 50.f * dt;
            }

            // auto aabb = dynamic_cast<AABB*>(s->ref.lock().get());
            // if(aabb)
            //     if(aabb->intersectAABB(other))
            //         cout << "COLLISION DETECTED YAY!!!\n";
        }
    }
};

void init(){
    assets::init();
    cout << "Assets init done.\n";
    viewport::init({960,720});
    cout << "Viewport init done.\n";
    debug::init();
    cout << "Debug init done.\n";
    factory::init();
    cout << "Node loader init done.\n";
    editor::init();

    srand(time(nullptr));

    debug::showAll();

    bool imgui_init_complete = ImGui::SFML::Init(viewport::wind);
    if(!imgui_init_complete){
        cout << "Note: couldn't init ImGUI in SFML window for some reason.\n";
    }
}

void exit(){
    viewport::exit();
}

int main(){
    init();

    sf::Clock dt_clock;
    sf::Time dt_time;
    float dt = 1.f/60.f;

    ImGuiIO& io = ImGui::GetIO();
    ImFont *def = io.Fonts->AddFontFromFileTTF("assets/main/fonts/courier.ttf", 20.f);

    bool result = ImGui::SFML::UpdateFontTexture();
    if(!result){
        cout << "Couldn't update font texture for some reason\n";
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.ChildRounding = 3.f;
    style.FrameRounding = 3.f;
    style.WindowRounding = 3.f;

    // cout << "loop begins...\n";

    //main loop
    while(viewport::wind.isOpen()){
        //update events
        while(const std::optional ev = viewport::wind.pollEvent()){
            ImGui::SFML::ProcessEvent(viewport::wind,*ev);
            if(ev->is<sf::Event::Closed>()){
                exit();
            }
            if(const auto* e = ev->getIf<sf::Event::MouseWheelScrolled>()){
                float d = e->delta;
                if(e->wheel == sf::Mouse::Wheel::Horizontal)
                    editor::wheel_delta_x = d;
                if(e->wheel == sf::Mouse::Wheel::Vertical)
                    editor::wheel_delta_y = d;
            }
        }
        updateControls();

        //physics
        // viewport::bg_color = editor::any_editor_window_focused ? sf::Color(63,63,0) : sf::Color::Black;

        viewport::cam_pos = editor::cam_pos;
        //render

        // cout << "node processing...\n";
        editor::node_root->process();
        // cout << "node drawing...\n";
        editor::node_root->draw();
        // cout << "node debug drawing...\n";
        editor::node_root->drawDebug();
        // cout << "displaying...\n";
        // cout << viewport::draw_queue.size();
        viewport::display(dt);

        ImGui::SFML::Update(viewport::wind, dt_time);
        // cout << "tree viewer processing...\n";
        // cout << "pushing font\n";
        ImGui::PushFont(def);
        // cout << "pushed font\n";
        // cout << "trying processing editor...\n";
        editor::process(dt);
        ImGui::PopFont();
        ImGui::SFML::Render(viewport::wind);
        viewport::wind.display();

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}