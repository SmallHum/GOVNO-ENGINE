#define EDITOR

#include <core/controls.h>
#include <core/assets.h>
#include <core/viewport.h>
#include <struct_loader.h>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
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

namespace editor{

    int id_fix;
    bool any_editor_window_focused = 0,
        controlling_camera = 0;

    v2f cam_pos = {-480.f, -360.f};

    float tree_view_width = 240,
        actions_width = 160,
        bar_height_button_size = 26,
        edit_width = 240,
        edit_pos_x;

    struct TreeNode : std::enable_shared_from_this<TreeNode>{
        weak_ptr<Node> ref;

        weak_ptr<TreeNode> parent;
        size_t parent_index = -1;

        bool collapsed = 1;
        bool is_active_selection = 0;
        bool is_multiple_selection = 0;
        size_t mult_sel_index = -1;

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

        void activeSelect(weak_ptr<TreeNode> &active_selection){
            if(auto a = active_selection.lock())
                a->is_active_selection = 0;
            active_selection = shared_from_this();
            is_active_selection = 1;
        }

        void activeUnselect(weak_ptr<TreeNode> &active_selection){
            active_selection.reset();
            is_active_selection = 0;
        }

        void multipleSelect(vector<weak_ptr<TreeNode>> &multiple_selection){
            mult_sel_index = multiple_selection.size();
            multiple_selection.push_back(shared_from_this());
            is_multiple_selection = 1;
        }

        void multipleUnselect(vector<weak_ptr<TreeNode>> &multiple_selection){
            multiple_selection.erase(
                multiple_selection.begin() + mult_sel_index
            );
            for(size_t i = mult_sel_index; i < multiple_selection.size(); i++)
                if(auto p = multiple_selection[i].lock())
                    multiple_selection[i].lock()->mult_sel_index--;
            is_multiple_selection = 0;
            mult_sel_index = -1;
        }

        void multipleUnselectAll(vector<weak_ptr<TreeNode>> &multiple_selection){
            for(auto &i : multiple_selection){
                auto p = i.lock();
                // cout << "NAME: " << p->ref.lock()->name << '\n';
                // cout << "MULT SEL INDEX: " << p->mult_sel_index << '\n';
                p->is_multiple_selection = 0;
                p->mult_sel_index = -1;
            }
            multiple_selection.clear();
        }

        void selectionProcess(weak_ptr<TreeNode> &active_selection, vector<weak_ptr<TreeNode>> &multiple_selection){

            if(ImGui::IsKeyDown(ImGuiKey_LeftShift)){
                // cout << "SHIFT SELECTION...\n";
                if(is_multiple_selection && is_active_selection){
                    // cout << "MULTIPLE AND ACTIVE\n";
                    multipleUnselect(multiple_selection);
                    activeUnselect(active_selection);
                }
                else if (is_multiple_selection && !is_active_selection){
                    // cout << "MULTIPLE BUT NOT ACTIVE\n";
                    activeSelect(active_selection);
                }
                else if (!is_multiple_selection && is_active_selection){
                    // cout << "ACTIVE BUT NOT MULTIPLE\n";
                    multipleSelect(multiple_selection);
                }
                else{
                    // cout << "NIETHER ACTIVE NOR MULTIPLE\n";
                    activeSelect(active_selection);
                    multipleSelect(multiple_selection);
                }
            }
            else {
                // cout << "NON SHIFT SELECTION WORKED\n";
                multipleUnselectAll(multiple_selection);
                multipleSelect(multiple_selection);
                activeSelect(active_selection);
            }
        }

        void drawCol1(weak_ptr<TreeNode> &active_selection, vector<weak_ptr<TreeNode>> &multiple_selection, float depth = 0){
            if(!ref.lock())
                return;

            ImGui::PushID(id_fix++);

            Node *n = ref.lock().get();

            ImGui::SetCursorPosX(depth*16.f + ImGui::GetCursorPosX());

            const char *button_icon = collapsed ? ">" : "v";

            if(is_active_selection){
                ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.2f, 0.3f, 0.5f, 1.0f});
            }
            else if(is_multiple_selection){
                ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.2f/2.f, 0.3f/2.f, 0.5f/2.f, 1.0f});
            }
            else{
                ImGui::PushStyleColor(ImGuiCol_ChildBg, {0.f, 0.f, 0.f, 1.f});
            }
            ImGui::BeginChild("MyBox", {200, 26}, ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);

            if(!children.empty()){
                if(ImGui::Button(button_icon,{26,26})){
                    collapsed = !collapsed;
                    selectionProcess(active_selection, multiple_selection);
                }
            }
            else{
                ImGui::InvisibleButton(".",{26,26});
            }
            ImGui::SameLine();

            ImGui::Text(n->name.c_str());
            if(ImGui::IsItemClicked()){
                selectionProcess(active_selection, multiple_selection);
            }

            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::PopID();

            if(!collapsed)
                for(auto &i : children)
                    i->drawCol1(active_selection, multiple_selection, depth+1);
        }

        void drawCol2(){
            if(!ref.lock())
                return;

            Node *n = ref.lock().get();
            // cout << parent << ' ' << n->name << '\n';

            ImGui::PushID(id_fix++);
            ImGui::Checkbox("visible",&n->visible);

            ImGui::PopID();

            if(!collapsed)
                for(auto &i : children)
                    i->drawCol2();
        }

        void del(){
            if(auto r = ref.lock()){
                if(auto p = r->parent.lock())
                    p->removeChild(r);
            }

            if(auto p = parent.lock()){
                auto this_iter = p->children.begin()+parent_index;
                for(auto i = this_iter+1; i != p->children.end(); i++){
                    (*i)->parent_index--;
                }
                p->children.erase(this_iter);
            }
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

    // Trying to implement undo/redo

    // typedef int data_t;

    // struct Action{
    //     Action(){execute();}
    //     virtual void execute();
    //     virtual void undo();
    // };

    // struct Paste: Action{
    //     data_t clipboard;
    //     vector<data_t>& data;
    //     vector<data_t>::iterator index;

    //     Paste(data_t clipboard, vector<data_t> &data, size_t index): 
    //     clipboard(clipboard),
    //     data(data),
    //     index(data.begin() + index){
    //         execute();
    //     }
    //     void execute() override{
    //         data.insert(index, clipboard);
    //     }
    //     void undo() override{
    //         data.erase(index);
    //     }
    // };

    // vector<shared_ptr<Action>> history;

    //////////////

    vector<weak_ptr<TreeNode>> to_remove;

    weak_ptr<TreeNode> active_selection;
    vector<weak_ptr<TreeNode>> multiple_selection;

    shared_ptr<TreeNode> tree_root;
    shared_ptr<Node> node_root;

    shared_ptr<Node> node_clipboard;

    //temporary
    // shared_ptr<Node> other;

    void init(){
        node_root = make_shared<Node>("root");
        node_gen::genTree(node_root, 2);

        // other = make_shared<AABB>();
        // node_root->addChild(other);

        node_root->printTree();

        editor::tree_root = make_shared<editor::TreeNode>();
        editor::tree_root->fill(node_root);
    }

    function<void()> nothing = [](){};

    void selAction(function<void()> &action, function<void()> &counter_action = nothing){
        if(auto s = active_selection.lock())action();
        else counter_action();
    }

    void newNode(function<shared_ptr<Node>()> &creator){
        if(auto s = active_selection.lock())active_selection.lock()->add(creator);
        else tree_root->add(creator);
    }

    function<void()> deleteNode = [](){
        to_remove = multiple_selection;
    };

    void open(){
        if(!node_root)return;

        active_selection.reset();
        multiple_selection.clear();

        node_root = constructFromFile("saved.ntr");
        tree_root->fill(node_root);
    }

    void save(){
        if(!node_root)return;
        node_root->writeToFile("saved.ntr");
    }

    void copy(){
        auto s = active_selection.lock()->ref;
        if(s.expired())return;
        node_clipboard = factory::copy(s);
    }

    void paste(){
        if(auto n = active_selection.lock()->ref.lock()){
            n->addChild(factory::copy(node_clipboard));
            active_selection.lock()->update();
            active_selection.lock()->show();
            active_selection.lock()->children.back()->multipleUnselectAll(multiple_selection);
            active_selection.lock()->children.back()->activeSelect(active_selection);
        }
    }

    void editFieldBool(string label, bool &v){
        ImGui::Checkbox(label.c_str(), &v);
    }

    void editFieldFloat(string label, float &v){
        float buffer = v;

        if(!ImGui::InputFloat(label.c_str(), &buffer))
            return;

        if(!ImGui::IsKeyPressed(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v = buffer;
    }

    void editFieldV2F(string label, v2f &v){
        float buffer[2] = {v.x, v.y};

        if(!ImGui::InputFloat2(label.c_str(), buffer))
            return;
        if(!ImGui::IsKeyPressed(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v.x = buffer[0];
        v.y = buffer[1];
    }

    void editFieldString(string label, string &v){
        string buffer = v;

        if(!ImGui::InputText("Name", &buffer))
            return;
        if(!ImGui::IsKeyPressed(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v.resize(strlen(buffer.c_str()));
        v = buffer;
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
    }

    void nodeEditMenu(Node *node){
        ImGui::SeparatorText("Node");

        editFieldString("Name", node->name);
        editFieldBool("Visible", node->visible);

        auto s = dynamic_cast<Spatial*>(node);
        if(s)spatialEditMenu(s);
    }

    void process(float &dt){
        id_fix = 0;

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
                if(ImGui::MenuItem("Delete Node", "Del"))selAction(deleteNode);
                ImGui::Separator();
                if(ImGui::MenuItem("Copy", "Ctrl+C"))copy();
                if(ImGui::MenuItem("Paste", "Ctrl+V"))paste();
                ImGui::Separator();
                if(ImGui::MenuItem("Move Camera", "Shift+F"))controlling_camera = 1;
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Keyboard actions.
        if(ImGui::IsKeyPressed(ImGuiKey_Delete))selAction(deleteNode);

        if(ImGui::IsKeyPressed(ImGuiKey_N) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )ImGui::OpenPopup("Make Node");

        if(ImGui::IsKeyPressed(ImGuiKey_C) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )copy();

        if(ImGui::IsKeyPressed(ImGuiKey_S) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )save();

        if(ImGui::IsKeyPressed(ImGuiKey_V) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )paste();

        if(ImGui::IsKeyPressed(ImGuiKey_O) && 
        (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) ||
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))
        )open();

        if(ImGui::IsKeyPressed(ImGuiKey_F) &&
            (ImGui::IsKeyDown(ImGuiKey_LeftShift) || 
            ImGui::IsKeyDown(ImGuiKey_RightShift)))controlling_camera = 1;

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

        ImGui::Columns(2);
        
        tree_root->drawCol1(active_selection, multiple_selection);
        ImGui::NextColumn();
        
        tree_root->drawCol2();

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


        if(auto sel = active_selection.lock())
            if(auto s = sel->ref.lock())
                nodeEditMenu(s.get());

        ImGui::End();

        // Make node popup.
        // Need to upgrade it.
        if(ImGui::BeginPopup("Make Node")){

            for(auto& i : factory::creators){
                if(ImGui::Button(factory::names[(size_t)i.first].c_str(),{64,26})){
                    newNode(i.second);
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }

        // Deletion process.
        // Due to crashes, deletion process is moved to the end of the function
        if(!to_remove.empty()){
            for(auto &i : to_remove)
                if(auto t = i.lock()){
                    t->del();
                }
            to_remove.clear();
            if(!multiple_selection.empty()){
                if(auto m = multiple_selection.front().lock())
                    m->multipleUnselectAll(multiple_selection);
            }
            if(auto a = active_selection.lock())
                a->activeUnselect(active_selection);
            multiple_selection.clear();
            active_selection.reset();
        }
        
        any_editor_window_focused = ImGui::IsWindowFocused(
            ImGuiFocusedFlags_AnyWindow | 
            ImGuiFocusedFlags_ChildWindows
        ) || ImGui::IsAnyItemFocused();

        if(any_editor_window_focused)return;

        if(controlling_camera){
            cam_pos += 600.f * dt * v2f(getDirHeld());
            if(ImGui::IsKeyPressed(ImGuiKey_Escape))
                controlling_camera = 0;
        }
        else if(auto s = active_selection.lock()){
            auto p = dynamic_cast<Spatial*>(s->ref.lock().get());
            if(p){
                p->pos += 150.f * dt * v2f(getDirHeld());
                if(ImGui::IsKeyDown(ImGuiKey_Q))p->angle += 50.f * dt;
                if(ImGui::IsKeyDown(ImGuiKey_W))p->angle -= 50.f * dt;
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

    GVEFont font1(getTexture("main:default_font"));
    GVEFont font2(getTexture("main:second_font"));
    string test_text_1 = "TEST FONT 1 ТЕСТ ПЕРВОГО ШРИФТА";
    string test_text_2 = "TEST FONT 2 ТЕСТ ВТОРОГО ШРИФТА";
    // string test_text_2 = "десятьбукв";

    cout << test_text_2.c_str() << '\n';

    for(auto &i : test_text_2){
        cout << (int)i << '\n';
    }

    for(int i = 0; i <= 255; i++){
        cout << (unsigned char)i << "   " << i << '\n';
    }

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
        // viewport::bg_color = editor::any_editor_window_focused ? sf::Color(63,63,0) : sf::Color::Black;

        viewport::cam_pos = editor::cam_pos;
        //render

        int j = 0;
        for(auto &i : test_text_1){
            viewport::draw(font1.getGlyph(i),0,0,3, mat3().translate({j * font1.char_size.x, 0.f}));
            j++;
        }
        j = 0;
        for(auto &i : test_text_2){
            viewport::draw(font2.getGlyph(i),0,0,3, mat3().translate({j * font2.char_size.x, 64.f}));
            j++;
        }

        // cout << "node processing...\n";
        editor::node_root->process();
        // cout << "node drawing...\n";
        editor::node_root->draw();
        // cout << "node debug drawing...\n";
        editor::node_root->drawDebug();
        // cout << "displaying...\n";
        viewport::display(dt);

        ImGui::SFML::Update(viewport::wind, dt_time);
        // cout << "tree viewer processing...\n";
        // cout << "pushing font\n";
        ImGui::PushFont(def);
        // cout << "pushed font\n";
        editor::process(dt);
        ImGui::PopFont();
        ImGui::SFML::Render(viewport::wind);
        viewport::wind.display();

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}