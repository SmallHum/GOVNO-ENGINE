#include <core/controls.h>
#include <core/viewport.h>
#include <core/assets.h>

#include <structs/spatial.h>
#include <fstream_opers.h>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

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

namespace tree_viewer{

    int id_fix;

    float tree_view_width = 240,
        actions_width = 160,
        bar_height_button_size = 26;

    string str_input(const char *label){
        string result;
        ImGui::InputText(label, result.data(), result.size());
        return result;
    }

    struct TreeNode : std::enable_shared_from_this<TreeNode>{
        weak_ptr<Node> ref;

        weak_ptr<TreeNode> parent;
        size_t parent_index = -1;

        bool collapsed = 1;

        vector<shared_ptr<TreeNode>> children;

        TreeNode(){}

        void fill(weak_ptr<Node> ref){
            children.clear();
            this->ref = ref;
            for(auto &i : ref.lock()->children){
                cout << "pushing child...\n";
                children.push_back(make_shared<TreeNode>());
                cout << "making shared from this...\n";
                children.back()->fill(i);
                children.back()->parent = shared_from_this();
                cout << "giving it a parent index...\n";
                children.back()->parent_index = children.size()-1;
            }
        }

        void update(){
            fill(ref);
        }

        void show(){
            collapsed = 0;
            if(auto p = parent.lock())
                p->show();
        }

        void drawCol1(weak_ptr<TreeNode> &selected, float depth = 0){
            if(!ref.lock())
                return;

            Node *n = ref.lock().get();

            ImGui::SetWindowPos({depth*16.f,bar_height_button_size});

            const char *button_icon = collapsed ? ">" : "v";
            
            ImGui::PushID(id_fix++);

            bool this_selected = selected.lock().get() == this;
            if(this_selected){
                ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.2f, 0.3f, 0.5f, 1.0f});
            }
            else{
                ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.f, 0.f, 0.f, 1.f});
            }
            ImGui::BeginChild("MyBox", {200, 26}, ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);

            if(!children.empty()){
                if(ImGui::Button(button_icon,{26,26})){
                    collapsed = !collapsed;
                    selected = shared_from_this();
                }
            }
            else{
                ImGui::InvisibleButton(".",{26,26});
            }
            ImGui::SameLine();

            ImGui::Text(n->name.c_str());
            if(ImGui::IsItemClicked()){
                selected = shared_from_this();
            }

            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::PopID();

            if(!collapsed)
                for(auto &i : children)
                    i->drawCol1(selected, depth+1);
        }

        void drawCol2(weak_ptr<TreeNode> &to_remove){
            if(!ref.lock())
                return;

            Node *n = ref.lock().get();
            // cout << parent << ' ' << n->name << '\n';

            ImGui::PushID(id_fix++);
            ImGui::Checkbox("visible",&n->visible);

            ImGui::SameLine();

            if(ImGui::Button("X",{24,24})){
                to_remove = shared_from_this();
            }
            ImGui::PopID();

            if(!collapsed)
                for(auto &i : children)
                    i->drawCol2(to_remove);
        }

        void del(){
            if(auto r = ref.lock()){
                if(auto p = r->parent.lock())
                    p->removeChild(r);
            }

            if(auto p = parent.lock())
                p->children.erase(p->children.begin()+parent_index);
        }

        void add(function<shared_ptr<Node>()> &creator){
            shared_ptr<Node> new_child = creator();
            ref.lock()->addChild(new_child);
            
            children.push_back(make_shared<TreeNode>());
            children.back()->fill(new_child);
            children.back()->parent = shared_from_this();
            children.back()->parent_index = children.size()-1;

            show();
        }
    };

    weak_ptr<TreeNode> to_remove;
    weak_ptr<TreeNode> selected;

    shared_ptr<TreeNode> tree_root;
    shared_ptr<Node> node_root;

    float scroll_y = 0.f;

    void init(){
        node_root = make_shared<Node>("root");
        node_gen::genTree(node_root, 3);
        node_root->printTree();

        tree_viewer::tree_root = make_shared<tree_viewer::TreeNode>();
        tree_viewer::tree_root->fill(node_root);
    }

    function<void()> nothing = [](){};

    void selAction(function<void()> &action, function<void()> &counter_action = nothing){
        if(auto s = selected.lock())action();
        else counter_action();
    }

    void newNode(function<shared_ptr<Node>()> &creator){
        if(auto s = selected.lock())selected.lock()->add(creator);
        else tree_root->add(creator);
    }

    function<void()> deleteNode = [](){
        to_remove = selected;
    };

    void open(){
        if(!node_root)return;

        node_root = constructFromFile("saved.ntr");
        tree_root->fill(node_root);
    }

    void save(){
        if(!node_root)return;
        node_root->writeToFile("saved.ntr");
    }

    void nodeEditMenu(weak_ptr<Node> node){
        // name
        // visible
        // active
    }

    void spatialEditMenu(weak_ptr<Node> node){
        nodeEditMenu(node);
        // pos
        // angle
        // scale
    }

    void process(float &dt){
        // cout << "idfix = 0...\n";
        id_fix = 0;

        // cout << "resetting...\n";
        to_remove.reset();

        // cout << "menu bar...\n";
        if(ImGui::BeginMainMenuBar()){

            if(ImGui::BeginMenu("File")){
                if(ImGui::MenuItem("Open", "Ctrl+O"))open();
                if(ImGui::MenuItem("Save", "Ctrl+S"))save();
                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Edit")){
                if(ImGui::MenuItem("New Node", "Ctrl+N"))ImGui::OpenPopup("Make Node");
                if(ImGui::MenuItem("Delete Node", "Del"))selAction(deleteNode);
                if(ImGui::MenuItem("Copy", "Ctrl+C"));
                if(ImGui::MenuItem("Paste", "Ctrl+V"));
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // cout << "keyboard actions...\n";
        if(ImGui::IsKeyPressed(ImGuiKey_Delete))selAction(deleteNode);

        if(ImGui::IsKeyPressed(ImGuiKey_N) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )ImGui::OpenPopup("Make Node");

        if(ImGui::IsKeyPressed(ImGuiKey_C) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        );

        if(ImGui::IsKeyPressed(ImGuiKey_S) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )save();

        if(ImGui::IsKeyPressed(ImGuiKey_V) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        );

        if(ImGui::IsKeyPressed(ImGuiKey_O) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )open();

        float height = viewport::wind.getSize().y-bar_height_button_size;

        // cout << "tree view window...\n";
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

        ImGui::Columns(2);
        
        cout << "draw col 1...\n";
        tree_root->drawCol1(selected);
        ImGui::NextColumn();
        
        cout << "draw col 2...\n";
        tree_root->drawCol2(to_remove);

        ImGui::End();

        if(ImGui::BeginPopup("Make Node")){

            for(auto& i : factory::creators){
                if(ImGui::Button(factory::names[(size_t)i.first].c_str(),{64,26})){
                    newNode(i.second);
                }
            }

            ImGui::EndPopup();
        }


        // ImGui::SetNextWindowPos({tree_view_width,0});
        // ImGui::SetNextWindowSize({actions_width,viewport::wind.getSize().y});
        // ImGui::SetNextWindowSizeConstraints({actions_width,viewport::wind.getSize().y},{FLT_MAX,FLT_MAX});
        // ImGui::Begin("Actions", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // node_maker::drawActions();

        // ImGui::End();

        if(auto t = to_remove.lock()){
            t->del();
        }

        if(auto s = selected.lock()){
            auto p = dynamic_cast<Spatial*>(s->ref.lock().get());
            if(p){
                p->pos += 150.f * dt * v2f(getDirHeld());
                if(ImGui::IsKeyDown(ImGuiKey_Q))p->angle += 50.f * dt;
                if(ImGui::IsKeyDown(ImGuiKey_W))p->angle -= 50.f * dt;
            }
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
    tree_viewer::init();

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

        // cout << "node processing...\n";
        tree_viewer::node_root->process();
        // cout << "node drawing...\n";
        tree_viewer::node_root->draw();
        // cout << "node debug drawing...\n";
        tree_viewer::node_root->drawDebug();
        // cout << "displaying...\n";
        viewport::display(dt);

        ImGui::SFML::Update(viewport::wind, dt_time);
        // cout << "tree viewer processing...\n";
        // cout << "pushing font\n";
        ImGui::PushFont(def);
        // cout << "pushed font\n";
        tree_viewer::process(dt);
        ImGui::PopFont();
        ImGui::SFML::Render(viewport::wind);
        viewport::wind.display();

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}